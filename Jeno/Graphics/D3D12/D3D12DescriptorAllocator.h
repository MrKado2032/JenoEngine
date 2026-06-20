#pragma once
#include "D3D12Common.h"
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
	struct DescriptorHandle : public Handle
	{
		JENO_NON_COPYABLE(DescriptorHandle)
		JENO_DEFAULT_MOVABLE(DescriptorHandle)

		friend class DescriptorAllocator;

		DescriptorHandle() noexcept = default;
		~DescriptorHandle() noexcept = default;

		[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const {return m_cpuHandle;}
		[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const {return m_gpuHandle;}

		void Reset() noexcept
		{
			Handle::Reset();

			m_cpuHandle = {};
			m_gpuHandle = {};
		}

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle{};
		D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle{};
	};
	
	class DescriptorHeap;
	class DescriptorAllocator
	{
	public:
		explicit DescriptorAllocator(DescriptorHeap& descriptorHeap);
		~DescriptorAllocator() noexcept = default;

		DescriptorAllocator(const DescriptorAllocator&) = delete;
		DescriptorAllocator& operator=(const DescriptorAllocator&) = delete;

		DescriptorAllocator(DescriptorAllocator&& other) noexcept = delete;
		DescriptorAllocator& operator=(DescriptorAllocator&& other) noexcept = delete;

		[[nodiscard]] DescriptorHandle Allocate();
		void Free(DescriptorHandle& handle);

	private:
		DescriptorHeap& m_descriptorHeap;

		std::vector<uint32_t> m_freeList;
	};
}