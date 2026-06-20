#pragma once

namespace Jeno::Graphics::D3D12
{
    class CommandQueue;
    class Device
    {
    public:
        Device();
        ~Device() noexcept = default;

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

        Device(Device&&) noexcept = delete;
        Device& operator=(Device&&) noexcept = delete;

        ID3D12Device5* GetNativeDevice() const {return m_d3d12Device.Get(); }
        IDXGIFactory7* GetDXGIFactory() const {return m_dxgiFactory.Get(); }

        CommandQueue& GetGraphicsQueue() const {return *m_graphicsQueue;}

    private:
        uint32_t SetupDebugMessenger();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> FindAdapter();
		D3D_FEATURE_LEVEL FindSupportedFeatureLevel(ID3D12Device* device, IDXGIAdapter4* adapter);
		void CreateDevice(const uint32_t dxgiFlags);

        Microsoft::WRL::ComPtr<ID3D12Device5> m_d3d12Device;
        Microsoft::WRL::ComPtr<IDXGIFactory7> m_dxgiFactory;

        std::unique_ptr<CommandQueue> m_graphicsQueue;

        D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_12_0;
    };
}