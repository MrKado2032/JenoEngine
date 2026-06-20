#pragma once

namespace Jeno::Graphics::D3D12
{
	class CommandContext;
	class Device;
	class CommandListManager
	{
	public:
		CommandListManager(const Device& device, uint32_t frameCount);
		~CommandListManager() noexcept = default;

		CommandListManager(const CommandListManager&) = delete;
		CommandListManager& operator=(const CommandListManager&) = delete;

		CommandListManager(CommandListManager&& other) noexcept = delete;
		CommandListManager& operator=(CommandListManager&& other) noexcept = delete;

		CommandContext& Begin(uint32_t index, uint64_t fenceValue);
		CommandContext& GetGraphicsContext(uint32_t i) const { return *m_graphicsContexts[i]; }

	private:
		std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_cmdAllocators;
		std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>> m_cmdLists;

		std::vector<std::unique_ptr<CommandContext>> m_graphicsContexts;
	};
}