#include "D3D12Pipeline.h"
#include "D3D12Helper.h"
#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12
{
    Pipeline::Pipeline(ID3D12Device* device, const PipelineCreateInfo& info)
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = Helper::CreateDefault2DDesc();
        psoDesc.pRootSignature = info.pipelineLayout.Get();
        psoDesc.VS.BytecodeLength = info.shader.vs.size();
        psoDesc.VS.pShaderBytecode = info.shader.vs.data();
        psoDesc.PS.BytecodeLength = info.shader.ps.size();
        psoDesc.PS.pShaderBytecode = info.shader.ps.data();
        psoDesc.InputLayout = { PipelineLayout::default2DInputElement, _countof(PipelineLayout::default2DInputElement) };

        JENO_THROW_IF_FAILED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
    }
}