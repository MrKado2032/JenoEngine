#pragma once

namespace Jeno::Graphics::D3D12
{
	class DescriptorHeap
	{
	public:
		DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, bool shaderVisible = false);
		~DescriptorHeap() noexcept = default;

		DescriptorHeap(const DescriptorHeap&) = delete;
		DescriptorHeap& operator=(const DescriptorHeap&) = delete;

		DescriptorHeap(DescriptorHeap&& other) noexcept = delete;
		DescriptorHeap& operator=(DescriptorHeap&& other) noexcept = delete;

		[[nodiscard]] ID3D12DescriptorHeap* Get() const { return m_descriptorHeap.Get(); }
		[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUStartHandle() const { return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(); }
		[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUStartHandle() const { return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(); }
		[[nodiscard]] uint32_t GetDescriptorSize() const noexcept { return m_descriptorSize; }
		[[nodiscard]] uint32_t GetNumDescriptors() const noexcept { return m_numDescriptors; }
		[[nodiscard]] bool IsShaderVisible() const noexcept { return m_shaderVisible; }

	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
		uint32_t m_descriptorSize = 0;
		uint32_t m_numDescriptors = 0;
		bool m_shaderVisible = false;
	};
}