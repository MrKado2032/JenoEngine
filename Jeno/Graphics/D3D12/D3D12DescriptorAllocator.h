#pragma once
#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12
{
	struct DescriptorHandle : public Handle
	{
		DescriptorHandle() noexcept = default;
		~DescriptorHandle() noexcept = default;

		DescriptorHandle(const DescriptorHandle&) = delete;
		DescriptorHandle& operator=(const DescriptorHandle&) = delete;

		DescriptorHandle(DescriptorHandle&&) noexcept = default;
		DescriptorHandle& operator=(DescriptorHandle&&) noexcept = default;

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle{};
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

		void Reset() noexcept
		{
			Handle::Reset();

			cpuHandle = {};
			gpuHandle = {};
		}
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