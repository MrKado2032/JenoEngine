#include "D3D12CommandQueue.h"

#include "D3D12Common.h" 

namespace Jeno::Graphics::D3D12
{
    CommandQueue::CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
	{
		if (!device)
		{
			throw std::runtime_error("D3D12 device is null");
		}

		CreateCommandQueue(device, type);
		CreateFence(device);
	}

	CommandQueue::~CommandQueue() noexcept
	{
		if (m_fenceEvent)
		{
			CloseHandle(m_fenceEvent);
			m_fenceEvent = nullptr;
		}
	}

	CommandQueue::CommandQueue(CommandQueue && other) noexcept
	{
		m_cmdQueue = std::move(other.m_cmdQueue);
		m_fence = std::move(other.m_fence);
		m_fenceEvent = std::exchange(other.m_fenceEvent, nullptr);
		m_nextFenceValue = std::exchange(other.m_nextFenceValue, 1);
	}

	CommandQueue& CommandQueue::operator=(CommandQueue && other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		if (other.m_fenceEvent)
		{
			CloseHandle(other.m_fenceEvent);
			m_fenceEvent = nullptr;
		}

		m_cmdQueue = std::move(other.m_cmdQueue);
		m_fence = std::move(other.m_fence);
		m_fenceEvent = std::exchange(other.m_fenceEvent, nullptr);
		m_nextFenceValue = std::exchange(other.m_nextFenceValue, 1);

		return *this;
	}

	void CommandQueue::Flush()
	{
		const auto fenceValue = Signal();
		WaitForFence(fenceValue);
	}

	void CommandQueue::WaitForFence(uint64_t fenceValue)
	{
		if (m_fence->GetCompletedValue() < fenceValue)
		{
			JENO_THROW_IF_FAILED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));

			const DWORD waitResult = WaitForSingleObject(m_fenceEvent, INFINITE);
			if (waitResult != WAIT_OBJECT_0)
			{
				throw std::runtime_error("failed to wait for fence event");
			}
		}
	}

	uint64_t CommandQueue::Signal()
	{
		const auto fenceValue = m_nextFenceValue++;
		JENO_THROW_IF_FAILED(m_cmdQueue->Signal(m_fence.Get(), fenceValue));

		return fenceValue;
	}

	void CommandQueue::ExecuteCommandList(ID3D12GraphicsCommandList* cmdList)
	{
		if (!cmdList)
		{
			throw std::runtime_error("Command list is nullptr");
		}

		ID3D12CommandList* cmdLists[] = { cmdList };
		m_cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	}

	void CommandQueue::CreateCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
	{
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
		cmdQueueDesc.Type = type;
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		cmdQueueDesc.NodeMask = 0;

		JENO_THROW_IF_FAILED(device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue)));
	}

	void CommandQueue::CreateFence(ID3D12Device* device)
	{
		JENO_THROW_IF_FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (!m_fenceEvent)
		{
			throw std::runtime_error("failed to create fence event");
		}
	}
}