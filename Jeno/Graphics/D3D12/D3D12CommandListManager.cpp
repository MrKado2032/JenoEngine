#include "D3D12CommandListManager.h"

#include "D3D12CommandContext.h"
#include "D3D12Common.h"
#include "D3D12Device.h"

namespace Jeno::Graphics::D3D12
{
	CommandListManager::CommandListManager(const Device& device, uint32_t frameCount)
	{
		if (frameCount == 0)
		{
			throw std::runtime_error("frame count must be greater than zero");
		}

		const auto ptrDevice = device.GetNativeDevice();
		if (!ptrDevice)
		{
			throw std::runtime_error("D3D12 device is null");
		}

		m_cmdAllocators.reserve(frameCount);
		m_cmdLists.reserve(frameCount);
		m_graphicsContexts.reserve(frameCount);

		for (uint32_t i = 0; i < frameCount; i++)
		{
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
			JENO_THROW_IF_FAILED(ptrDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator)));

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1> cmdList;
			JENO_THROW_IF_FAILED(ptrDevice->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&cmdList)));

			m_cmdAllocators.push_back(cmdAllocator);
			m_cmdLists.push_back(cmdList);

			m_graphicsContexts.push_back(std::make_unique<CommandContext>(cmdList.Get(), cmdAllocator.Get(), device.GetGraphicsQueue()));
		}
	}

	CommandContext& CommandListManager::Begin(uint32_t index, uint64_t fenceValue)
	{
		if (index >= m_graphicsContexts.size())
		{
			throw std::runtime_error("command context index is out of range");
		}

		auto& ctx = m_graphicsContexts[index];
		if (fenceValue != 0)
		{
			ctx->WaitForFence(fenceValue);
		}

		return ctx->Begin();
	}
}