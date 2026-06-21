#include "D3D12PipelineLayout.h"
#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12
{
    PipelineLayout::PipelineLayout(ID3D12Device* device)
    {
		CD3DX12_DESCRIPTOR_RANGE1 ranges[1]{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE);

		CD3DX12_ROOT_PARAMETER1 rootParams[3]{};
		rootParams[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_VERTEX);
        rootParams[1].InitAsConstantBufferView(1, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_PIXEL);
		rootParams[2].InitAsDescriptorTable(_countof(ranges), ranges, D3D12_SHADER_VISIBILITY_PIXEL);

		CD3DX12_STATIC_SAMPLER_DESC samplerDescs[1]{};
		samplerDescs[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc{ };
		rootSigDesc.Init_1_1(_countof(rootParams), rootParams, _countof(samplerDescs), samplerDescs, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		Microsoft::WRL::ComPtr<ID3DBlob> rootsig, error;
		JENO_THROW_IF_FAILED(D3DX12SerializeVersionedRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &rootsig, &error));
		JENO_THROW_IF_FAILED(device->CreateRootSignature(0, rootsig->GetBufferPointer(), rootsig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
    }
}