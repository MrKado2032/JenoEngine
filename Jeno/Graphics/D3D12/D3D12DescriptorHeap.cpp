#include "D3D12DescriptorHeap.h"

#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12
{
	DescriptorHeap::DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, bool shaderVisible) : m_numDescriptors(numDescriptors), m_shaderVisible(shaderVisible)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = static_cast<UINT>(numDescriptors);
		heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;

		JENO_THROW_IF_FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descriptorHeap)));

		m_descriptorSize = static_cast<uint32_t>(device->GetDescriptorHandleIncrementSize(type));
	}
}
