#include "D3D12CommandContext.h"

#include "D3D12Common.h"
#include "D3D12CommandQueue.h"

namespace Jeno::Graphics::D3D12
{
	CommandContext::CommandContext(ID3D12GraphicsCommandList1* cmdList, ID3D12CommandAllocator* cmdAlloc, CommandQueue& cmdQueue)
		: m_cmdList(cmdList), m_cmdAllocator(cmdAlloc), m_cmdQueue(cmdQueue)
	{
		if (!m_cmdList)
		{
			throw std::runtime_error("command list is null");
		}

		if (!m_cmdAllocator)
		{
			throw std::runtime_error("command allocator is null");
		}
	}

	CommandContext::~CommandContext() noexcept
	{
	
	}

	void CommandContext::Reset()
	{
		JENO_THROW_IF_FAILED(m_cmdAllocator->Reset());
		JENO_THROW_IF_FAILED(m_cmdList->Reset(m_cmdAllocator, nullptr));
	}

	void CommandContext::Close()
	{
		JENO_THROW_IF_FAILED(m_cmdList->Close());
	}

	void CommandContext::WaitForFence(uint64_t fenceValue)
	{
		m_cmdQueue.WaitForFence(fenceValue);
	}

	uint64_t CommandContext::Submit()
	{
		Close();

		m_cmdQueue.ExecuteCommandList(m_cmdList);
		return m_cmdQueue.Signal();
	}

	CommandContext& CommandContext::Begin()
	{
		Reset();

		return *this;
	}

	void CommandContext::TransitionBarrier(ID3D12Resource* res, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(res, beforeState, afterState);
		m_cmdList->ResourceBarrier(1, &barrier);
	}

	void CommandContext::SetViewPort(const D3D12_VIEWPORT& viewport)
	{
		m_cmdList->RSSetViewports(1, &viewport);
	}

	void CommandContext::SetScissor(const D3D12_RECT& scissor)
	{
		m_cmdList->RSSetScissorRects(1, &scissor);
	}

	void CommandContext::SetPipeline(ID3D12PipelineState* pipeline)
	{
		m_cmdList->SetPipelineState(pipeline);
	}

	void CommandContext::SetRootSignature(ID3D12RootSignature* rootSignature)
	{
		m_cmdList->SetGraphicsRootSignature(rootSignature);
	}

	void CommandContext::SetPrimitiveToplogy(D3D12_PRIMITIVE_TOPOLOGY topology)
	{
		m_cmdList->IASetPrimitiveTopology(topology);
	}

	void CommandContext::SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW & vertView)
	{
		m_cmdList->IASetVertexBuffers(0, 1, &vertView);
	}

	void CommandContext::SetIndexBufferView(const D3D12_INDEX_BUFFER_VIEW& indexView)
	{
		m_cmdList->IASetIndexBuffer(&indexView);
	}

	void CommandContext::SetConstantBufferView(uint32_t rootParamIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
	{
		m_cmdList->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	}

	void CommandContext::SetRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle)
	{
		m_cmdList->OMSetRenderTargets(1, &cpuHandle, FALSE, nullptr);
	}

	void CommandContext::ClearRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle)
	{
		const FLOAT clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		m_cmdList->ClearRenderTargetView(cpuHandle, clearColor, 0, nullptr);
	}

	void CommandContext::DrawIndexed(uint32_t indexCount, uint32_t instanceCount)
	{
		m_cmdList->DrawIndexedInstanced(static_cast<UINT>(indexCount), static_cast<UINT>(instanceCount), 0, 0, 0);
	}
}