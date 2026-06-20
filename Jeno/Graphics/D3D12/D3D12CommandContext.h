#pragma once

namespace Jeno::Graphics::D3D12
{
	class CommandQueue;
	class CommandContext
	{
	public:
		CommandContext(ID3D12GraphicsCommandList1* cmdList, ID3D12CommandAllocator* cmdAlloc, CommandQueue& cmdQueue);
		~CommandContext() noexcept;

		CommandContext(const CommandContext&) = delete;
		CommandContext& operator=(const CommandContext&) = delete;

		CommandContext(CommandContext&& other) noexcept = delete;
		CommandContext& operator=(CommandContext&& other) noexcept = delete;

		void Reset();
		void Close();
		void WaitForFence(uint64_t fenceValue);
		uint64_t Submit();

		CommandContext& Begin();

		void TransitionBarrier(ID3D12Resource* res, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);
		void SetViewPort(const D3D12_VIEWPORT& viewport);
		void SetScissor(const D3D12_RECT& scissor);
		void SetPipeline(ID3D12PipelineState* pipeline);
		void SetRootSignature(ID3D12RootSignature* rootSignature);
		void SetPrimitiveToplogy(D3D12_PRIMITIVE_TOPOLOGY topology);
		void SetVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW& vertView);
		void SetIndexBufferView(const D3D12_INDEX_BUFFER_VIEW& indexView);
		void SetConstantBufferView(uint32_t rootParamIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
		void SetRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
		void ClearRenderTarget(const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle);
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount);

	private:
		ID3D12GraphicsCommandList1* m_cmdList = nullptr;
		ID3D12CommandAllocator* m_cmdAllocator = nullptr;

		CommandQueue& m_cmdQueue;
	};
}