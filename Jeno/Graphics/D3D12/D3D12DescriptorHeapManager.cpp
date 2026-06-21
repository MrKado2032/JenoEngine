#include "D3D12DescriptorHeapManager.h"

#include "D3D12DescriptorHeap.h"
#include "D3D12DescriptorAllocator.h"

namespace Jeno::Graphics::D3D12
{
	DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* device)
	{
		m_rtvHeap = std::make_unique<DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
		m_rtvAllocator = std::make_unique<DescriptorAllocator>(*m_rtvHeap);

		m_srvHeap = std::make_unique<DescriptorHeap>(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1024, true);
		m_srvAllocator = std::make_unique<DescriptorAllocator>(*m_srvHeap);
	}
	DescriptorHeapManager::~DescriptorHeapManager() noexcept = default;
}