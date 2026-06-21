#pragma once

namespace Jeno::Graphics::D3D12
{
	class DescriptorAllocator;
	class DescriptorHeap;
	class DescriptorHeapManager
	{
	public:
		DescriptorHeapManager(ID3D12Device* device);
		~DescriptorHeapManager() noexcept;

		DescriptorHeapManager(const DescriptorHeapManager&) = delete;
		DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

		DescriptorHeapManager(DescriptorHeapManager&&) noexcept = delete;
		DescriptorHeapManager& operator=(DescriptorHeapManager&&) noexcept = delete;

		DescriptorHeap& GetSRVHeap() const { return *m_srvHeap; }

		DescriptorAllocator& GetRTVAllocator() const { return *m_rtvAllocator; }
		DescriptorAllocator& GetSRVAllocator() const { return *m_srvAllocator; }

	private:
		std::unique_ptr<DescriptorHeap> m_rtvHeap;
		std::unique_ptr<DescriptorAllocator> m_rtvAllocator;

		std::unique_ptr<DescriptorHeap> m_srvHeap;
		std::unique_ptr<DescriptorAllocator> m_srvAllocator;

	};
}