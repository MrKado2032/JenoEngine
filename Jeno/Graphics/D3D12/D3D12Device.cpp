#include "D3D12Device.h"
#include "D3D12Common.h"

#include "D3D12CommandQueue.h"

namespace Jeno::Graphics::D3D12
{
    Device::Device()
    {
		uint32_t dxgiFlags = 0;
#ifdef _DEBUG
		dxgiFlags = SetupDebugMessenger();
#endif
		CreateDevice(dxgiFlags);

        m_graphicsQueue = std::make_unique<CommandQueue>(m_d3d12Device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
    }

    uint32_t Device::SetupDebugMessenger()
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            return DXGI_CREATE_FACTORY_DEBUG;
        }

        return 0;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter4> Device::FindAdapter()
    {
        for (UINT i = 0; ; i++)
        {
            Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
            HRESULT hr = m_dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
            if (hr == DXGI_ERROR_NOT_FOUND)
            {
                return nullptr;
            }

            DXGI_ADAPTER_DESC3 adpDesc{};
            adapter->GetDesc3(&adpDesc);

            // Skip software adapter
            if (adpDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) continue;

            return adapter;
        }

        return nullptr;
    }

    D3D_FEATURE_LEVEL Device::FindSupportedFeatureLevel(ID3D12Device* device, IDXGIAdapter4* adapter)
    {
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_12_2,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS supportedFeatureLevels{ .NumFeatureLevels = _countof(featureLevels), .pFeatureLevelsRequested = featureLevels };

        JENO_THROW_IF_FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &supportedFeatureLevels, sizeof(supportedFeatureLevels)));

        if (supportedFeatureLevels.MaxSupportedFeatureLevel < D3D_FEATURE_LEVEL_12_0)
        {
            throw std::runtime_error("D3D feature level 12_0 is required !");
        }

        return supportedFeatureLevels.MaxSupportedFeatureLevel;
    }

    void Device::CreateDevice(const uint32_t dxgiFlags)
    {
        JENO_THROW_IF_FAILED(CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&m_dxgiFactory)));

        Microsoft::WRL::ComPtr<IDXGIAdapter4> foundAdapter = FindAdapter();

        JENO_THROW_IF_FAILED(D3D12CreateDevice(foundAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_d3d12Device)));

        m_featureLevel = FindSupportedFeatureLevel(m_d3d12Device.Get(), foundAdapter.Get());
    }
}