#pragma once
#include "D3D12DescriptorAllocator.h"

namespace Jeno::Graphics::D3D12
{
	class Device;
	class DescriptorAllocator;
	class Swapchain
	{
	public:
		static constexpr uint32_t kFrameCount = 2;

		Swapchain(Device& device, DescriptorAllocator& rtvAllocator, uint32_t width, uint32_t height, HWND hWnd);
		~Swapchain() noexcept = default;

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;

		Swapchain(Swapchain&&) noexcept = delete;
		Swapchain& operator=(Swapchain&&) noexcept = delete;

		struct FrameContext
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			DescriptorHandle handle{};
		};

		uint32_t GetCurrentBackBufferIdx() const { return m_swapchain->GetCurrentBackBufferIndex(); }
		const FrameContext& GetCurrentFrameContext() const { return m_frames[GetCurrentBackBufferIdx()]; }

		uint32_t GetCurrentWidth() const { return m_currentWidth; }
		uint32_t GetCurrentHeight() const { return m_currentHeight; }

		void Present(bool useVsync = true);
		void Resize(uint32_t width, uint32_t height);

	private:

		void CreateSwapchain(Device& device, uint32_t width, uint32_t height, HWND hWnd);
		void CreateRTV(Device& device, DescriptorAllocator& rtvAllocator);

		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;
		std::array<FrameContext, static_cast<size_t>(kFrameCount)> m_frames;

		uint32_t m_currentWidth = 0;
		uint32_t m_currentHeight = 0;

		Device& m_device;
		DescriptorAllocator& m_rtvAllocator;
	};
}