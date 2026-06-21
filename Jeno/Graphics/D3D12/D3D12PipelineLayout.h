#pragma once 
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
    class PipelineLayout
    {
    public:
        JENO_DEFAULT_MOVABLE(PipelineLayout)
        PipelineLayout(ID3D12Device* device);
        ~PipelineLayout() noexcept = default;

        ID3D12RootSignature* Get() const { return m_rootSignature.Get(); }
        
        static constexpr D3D12_INPUT_ELEMENT_DESC default2DInputElement[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};
    private:
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    };
}