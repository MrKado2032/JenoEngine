#include "D3D12Renderer.h"

#include "Core/Window.h"
#include "D3D12Device.h"
#include "D3D12DescriptorHeapManager.h"
#include "D3D12CommandListManager.h"
#include "D3D12Swapchain.h"
#include "D3D12CommandQueue.h"
#include "D3D12CommandContext.h"

#include "Core/ShaderCompiler.h"

#include "D3D12Mesh.h"
#include "D3D12Material.h"
#include "D3D12Camera.h"
#include "D3D12TextureManager.h"
#include "D3D12Transform.h"
#include "D3D12ShaderTypes.h"
#include "D3D12DescriptorHeap.h"
#include "D3D12PipelineLayout.h"
#include "D3D12Pipeline.h"
#include "D3D12SpriteRenderer.h"

namespace Jeno::Graphics::D3D12
{
	Renderer::Renderer(Jeno::Core::Window& window)
	{
		m_device = std::make_unique<Device>();
		
		m_descriptorHeapManager = std::make_unique<DescriptorHeapManager>(m_device->GetNativeDevice());
		
		m_commandListManager = std::make_unique<CommandListManager>(*m_device, Swapchain::kFrameCount);

		m_textureManager = std::make_unique<TextureManager>(*m_device, *m_descriptorHeapManager);
		
		m_swapchain = std::make_unique<Swapchain>(*m_device, m_descriptorHeapManager->GetRTVAllocator(), window.GetWidth(), window.GetHeight(), window.GetWindowHandle());

		m_frames.resize(static_cast<size_t>(Swapchain::kFrameCount));

		// RootSignature
		m_pipelineLayout = std::make_unique<PipelineLayout>(m_device->GetNativeDevice());
		
		// Shader Compile
		auto shaderSet = Core::ShaderCompiler::Compile("Assets/Shaders/Basic2DVS.cso", "Assets/Shaders/Basic2DPS.cso");

		// PSO
		PipelineCreateInfo pipelineCI{.pipelineLayout = *m_pipelineLayout, .shader = shaderSet};

		m_pipeline = std::make_unique<Pipeline>(m_device->GetNativeDevice(), pipelineCI);
		
		m_camera = std::make_unique<Camera>(window.GetWidth(), window.GetHeight());
		
		// Sample Texture
		m_defaultTexHandle = m_textureManager->LoadTexture(L"Assets/textures/jeno-white.png");

		m_spriteRenderer = std::make_unique<SpriteRenderer>(*this);
	}

	Renderer::~Renderer() noexcept
	{
		m_device->GetGraphicsQueue().Flush();
	}

	void Renderer::BeginFrame()
	{
		const auto index = m_swapchain->GetCurrentBackBufferIdx();
		const auto& frame = m_frames[index];

		auto& ctx = m_commandListManager->Begin(index, frame.fenceValue);

		auto& backBufferResource = m_swapchain->GetCurrentFrameContext();
		ctx.TransitionBarrier(backBufferResource.resource.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		ctx.SetViewPort(CD3DX12_VIEWPORT({ 0, 0, static_cast<FLOAT>(m_swapchain->GetCurrentWidth()), static_cast<FLOAT>(m_swapchain->GetCurrentHeight()) }));
		ctx.SetScissor(CD3DX12_RECT(0, 0, static_cast<LONG>(m_swapchain->GetCurrentWidth()), static_cast<LONG>(m_swapchain->GetCurrentHeight())));

		ctx.SetRootSignature(m_pipelineLayout->Get());
		ctx.SetPipeline(m_pipeline->Get());
		
		ctx.SetRenderTarget(backBufferResource.handle.GetCPUHandle());
		ctx.ClearRenderTarget(backBufferResource.handle.GetCPUHandle());
	}

	void Renderer::EndFrame()
	{
		const auto index = m_swapchain->GetCurrentBackBufferIdx();
		auto& frame = m_frames[index];

		auto& ctx = m_commandListManager->GetGraphicsContext(index);

		auto& backBufferResource = m_swapchain->GetCurrentFrameContext();
		ctx.TransitionBarrier(backBufferResource.resource.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		frame.fenceValue = ctx.Submit();

		m_swapchain->Present();
	}

	void Renderer::DrawMesh(const Mesh& mesh, const Material& material, const Transform& transform, ConstantBuffer<TransformCB>& consBuffer)
	{
		auto& ctx = m_commandListManager->GetGraphicsContext(m_swapchain->GetCurrentBackBufferIdx());

		ctx.SetDescriptorHeap(m_descriptorHeapManager->GetSRVHeap().Get());

		// --- Use the default texture if the material has no valid texture.

		const auto texHandle = material.GetTextureHandle().IsValid() ? material.GetTextureHandle() : m_defaultTexHandle;
		const auto& texResource = m_textureManager->GetTextureFromHandle(texHandle);
		ctx.SetGraphicsRootDescriptorTable(2, texResource.descriptorHandle.GetGPUHandle());
		
		// ---

		TransformCB transformCB{};
		DirectX::XMStoreFloat4x4(&transformCB.mvp, DirectX::XMMatrixTranspose(transform.GetWorld() * m_camera->GetViewProjMatrix()));

		consBuffer.Update(transformCB);

		ctx.SetConstantBufferView(0, consBuffer.GetVirtualAddress());
        ctx.SetConstantBufferView(1, material.GetColorCB().GetVirtualAddress());

		ctx.SetPrimitiveToplogy(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx.SetVertexBufferView(mesh.GetVertView());
		ctx.SetIndexBufferView(mesh.GetIndexView());
		ctx.DrawIndexed(mesh.GetIndexCount(), 1);
	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{
		m_swapchain->Resize(width, height);	
	}
}