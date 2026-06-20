#include "D3D12Renderer.h"

#include "Core/Window.h"
#include "D3D12Device.h"
#include "D3D12DescriptorHeapManager.h"
#include "D3D12CommandListManager.h"
#include "D3D12Swapchain.h"
#include "D3D12CommandQueue.h"
#include "D3D12CommandContext.h"

#include "Core/ShaderCompiler.h"

#include "D3D12Helper.h"

#include "D3D12Mesh.h"
#include "D3D12Material.h"
#include "D3D12Camera.h"
#include "D3D12Transform.h"
#include "D3D12ShaderTypes.h"

namespace Jeno::Graphics::D3D12
{
	Renderer::Renderer(Jeno::Core::Window& window)
	{
		m_device = std::make_unique<Device>();
		
		m_descriptorHeapManager = std::make_unique<DescriptorHeapManager>(m_device->GetNativeDevice());
		
		m_commandListManager = std::make_unique<CommandListManager>(*m_device, Swapchain::kFrameCount);
		
		m_swapchain = std::make_unique<Swapchain>(*m_device, m_descriptorHeapManager->GetRTVAllocator(), window.GetWidth(), window.GetHeight(), window.GetWindowHandle());

		m_frames.resize(static_cast<size_t>(Swapchain::kFrameCount));

		// RootSignature
		CD3DX12_ROOT_PARAMETER1 rootParams[2]{};
		rootParams[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX);
        rootParams[1].InitAsConstantBufferView(1, 0, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, D3D12_SHADER_VISIBILITY_VERTEX);

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc{ };
		rootSigDesc.Init_1_1(_countof(rootParams), rootParams, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		Microsoft::WRL::ComPtr<ID3DBlob> rootsig, error;
		JENO_THROW_IF_FAILED(D3DX12SerializeVersionedRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &rootsig, &error));
		JENO_THROW_IF_FAILED(m_device->GetNativeDevice()->CreateRootSignature(0, rootsig->GetBufferPointer(), rootsig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));

		D3D12_INPUT_ELEMENT_DESC inputElements[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		// Shader Compile
		auto shaderSet = Core::ShaderCompiler::Compile("Assets/Shaders/Basic2DVS.cso", "Assets/Shaders/Basic2DPS.cso");

		// PSO
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = Helper::CreateDefault2DDesc();
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS.BytecodeLength = shaderSet.vs.size();
		psoDesc.VS.pShaderBytecode = shaderSet.vs.data();
		psoDesc.PS.BytecodeLength = shaderSet.ps.size();
		psoDesc.PS.pShaderBytecode = shaderSet.ps.data();
		psoDesc.InputLayout = { inputElements, _countof(inputElements) };

		JENO_THROW_IF_FAILED(m_device->GetNativeDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
		
		m_camera = std::make_unique<Camera>(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));
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

		ctx.SetRootSignature(m_rootSignature.Get());
		ctx.SetPipeline(m_pipelineState.Get());
		
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

		TransformCB transformCB{};
		DirectX::XMStoreFloat4x4(&transformCB.mvp, XMMatrixTranspose(transform.GetWorld() * m_camera->GetViewProjMatrix()));

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