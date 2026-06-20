#pragma once

namespace Jeno::Graphics::D3D12::Helper
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDefault2DDesc();

	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, size_t size, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer);
}