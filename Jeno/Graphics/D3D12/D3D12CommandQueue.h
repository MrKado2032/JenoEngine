#pragma once

namespace Jeno::Graphics::D3D12
{
    class Device;
    class CommandQueue
	{
	public:
		CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
		~CommandQueue() noexcept;

		CommandQueue(const CommandQueue&) = delete;
		CommandQueue& operator=(const CommandQueue&) = delete;

		CommandQueue(CommandQueue&& other) noexcept;
		CommandQueue& operator=(CommandQueue&& other) noexcept;

		void Flush();
		void WaitForFence(uint64_t fenceValue);
		uint64_t Signal();

		void ExecuteCommandList(ID3D12GraphicsCommandList* cmdList);

		ID3D12CommandQueue* Get() const noexcept { return m_cmdQueue.Get(); }

	private:
		void CreateCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
		void CreateFence(ID3D12Device* device);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_cmdQueue;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		HANDLE m_fenceEvent = nullptr;
		uint64_t m_nextFenceValue = 1;
	};
}