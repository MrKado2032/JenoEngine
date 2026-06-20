#include "D3D12Helper.h"

#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12::Helper
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreateDefault2DDesc()
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
		psoDesc.pRootSignature = nullptr;
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.NodeMask = 0;
		psoDesc.NumRenderTargets = 1;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.SampleMask = UINT_MAX;

		return psoDesc;
	}

	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, size_t size, Microsoft::WRL::ComPtr<ID3D12Resource>& buffer)
	{
		auto heapProps = CD3DX12_HEAP_PROPERTIES(type);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT64>(size));

		const auto initialState =
			type == D3D12_HEAP_TYPE_UPLOAD
			? D3D12_RESOURCE_STATE_GENERIC_READ
			: D3D12_RESOURCE_STATE_COMMON;

		JENO_THROW_IF_FAILED(device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resDesc, initialState, nullptr, IID_PPV_ARGS(&buffer)));
	}
}