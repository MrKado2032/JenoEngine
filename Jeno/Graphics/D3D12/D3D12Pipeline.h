#pragma once
#include "Core/ShaderCompiler.h"
#include "D3D12PipelineLayout.h"
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
    struct PipelineCreateInfo
    {
        PipelineLayout& pipelineLayout;
        Core::ShaderSet shader{};
    };

    class Pipeline
    {
    public:
        JENO_MOVE_ONLY(Pipeline)
        
        Pipeline(ID3D12Device* device, const PipelineCreateInfo& info);
        ~Pipeline() noexcept = default;

        ID3D12PipelineState* Get() const { return m_pipelineState.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    };
}