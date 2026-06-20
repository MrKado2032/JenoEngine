#include "D3D12Swapchain.h"

#include "D3D12Common.h"
#include "D3D12Device.h"
#include "D3D12CommandQueue.h"

namespace Jeno::Graphics::D3D12
{
	Swapchain::Swapchain(Device& device, DescriptorAllocator& rtvAllocator, uint32_t width, uint32_t height, HWND hWnd) 
		: m_device(device), m_rtvAllocator(rtvAllocator), m_currentWidth(width), m_currentHeight(height)
	{
		CreateSwapchain(device, width, height, hWnd);
		CreateRTV(device, rtvAllocator);
	}

	void Swapchain::Present(bool useVsync)
	{
		JENO_THROW_IF_FAILED(m_swapchain->Present(useVsync ? 1 : 0, 0));
	}

	void Swapchain::Resize(uint32_t width, uint32_t height)
	{
		m_device.GetGraphicsQueue().Flush();

		for (auto& frame : m_frames)
		{
			frame.resource.Reset();
			m_rtvAllocator.Free(frame.handle);
		}

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
		JENO_THROW_IF_FAILED(m_swapchain->GetDesc1(&swapchainDesc));
		JENO_THROW_IF_FAILED(m_swapchain->ResizeBuffers(swapchainDesc.BufferCount, static_cast<UINT>(width), static_cast<UINT>(height), swapchainDesc.Format, swapchainDesc.Flags));

		CreateRTV(m_device, m_rtvAllocator);

		m_currentWidth = width;
		m_currentHeight = height;
	}

	void Swapchain::CreateSwapchain(Device& device, uint32_t width, uint32_t height, HWND hWnd)
	{
		const auto dxgiFactory = device.GetDXGIFactory();
		const auto& gfxQueue = device.GetGraphicsQueue();

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
		swapchainDesc.Width = static_cast<UINT>(width);
		swapchainDesc.Height = static_cast<UINT>(height);
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapchainDesc.BufferCount = static_cast<UINT>(kFrameCount);
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.Flags = 0;
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapchainDesc.Stereo = FALSE;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;
		JENO_THROW_IF_FAILED(dxgiFactory->CreateSwapChainForHwnd(gfxQueue.Get(), hWnd, &swapchainDesc, nullptr, nullptr, &swapchain1));
		JENO_THROW_IF_FAILED(swapchain1.As(&m_swapchain));
	}

	void Swapchain::CreateRTV(Device& device, DescriptorAllocator& rtvAllocator)
	{
		const auto ptrDevice = device.GetNativeDevice();

		for (UINT i = 0; i < kFrameCount; i++)
		{
			m_frames[i].handle = rtvAllocator.Allocate();

			JENO_THROW_IF_FAILED(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_frames[i].resource)));
			ptrDevice->CreateRenderTargetView(m_frames[i].resource.Get(), nullptr, m_frames[i].handle.cpuHandle);
		}
	}
}