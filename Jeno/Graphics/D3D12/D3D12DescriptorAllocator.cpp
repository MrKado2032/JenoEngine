#include "D3D12DescriptorAllocator.h"

#include "D3D12DescriptorHeap.h"

namespace Jeno::Graphics::D3D12
{
	DescriptorAllocator::DescriptorAllocator(DescriptorHeap& descriptorHeap) : m_descriptorHeap(descriptorHeap)
	{
		const auto count = descriptorHeap.GetNumDescriptors();

		m_freeList.resize(static_cast<size_t>(count));
		for (uint32_t i = 0; i < count; i++)
		{
			m_freeList[i] = count - i - 1;
		}
	}

	DescriptorHandle DescriptorAllocator::Allocate()
	{
		if (m_freeList.empty())
		{
			throw std::runtime_error("descriptor allocator is out of free descriptors");
		}

		auto const index = m_freeList.back();
		m_freeList.pop_back();

		const auto startCpuHandle = m_descriptorHeap.GetCPUStartHandle();

		const auto offset = static_cast<SIZE_T>(index) * m_descriptorHeap.GetDescriptorSize();

		DescriptorHandle handle{};
		handle.m_cpuHandle.ptr = startCpuHandle.ptr + offset;

		handle.m_gpuHandle.ptr = 0;
		if (m_descriptorHeap.IsShaderVisible())
		{
			const auto startGpuHandle = m_descriptorHeap.GetGPUStartHandle();
			const auto gpuOffset = static_cast<UINT64>(index) * m_descriptorHeap.GetDescriptorSize();
			handle.m_gpuHandle.ptr = startGpuHandle.ptr + gpuOffset;
		}

		handle.index = index;

		return handle;
	}

	void DescriptorAllocator::Free(DescriptorHandle& handle)
	{
		if (!handle.IsValid())
		{
			return;
		}

		if (handle.index >= m_descriptorHeap.GetNumDescriptors())
		{
			throw std::runtime_error("invalid descriptor handle index");
		}
		
		m_freeList.push_back(handle.index);

		handle.Reset();
	}
}