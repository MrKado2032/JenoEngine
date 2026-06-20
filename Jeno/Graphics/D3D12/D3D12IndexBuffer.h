#pragma once
#include "D3D12Buffer.h"
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
	class IndexBuffer
	{
	public:
        JENO_DECLARE_MOVE_ONLY(IndexBuffer)

		IndexBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const uint16_t> data);
		~IndexBuffer() noexcept = default;

		Buffer<uint16_t>& Get() const { return *m_indexBuffer; }
		D3D12_INDEX_BUFFER_VIEW GetView() const { return m_indexView; }
		uint32_t GetIndexCount() const { return m_indexCount; }

	private:
		std::unique_ptr<Buffer<uint16_t>> m_indexBuffer;
		D3D12_INDEX_BUFFER_VIEW m_indexView{};
		uint32_t m_indexCount = 0;
	};
}