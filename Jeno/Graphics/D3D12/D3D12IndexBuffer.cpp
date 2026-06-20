#include "D3D12IndexBuffer.h"

namespace Jeno::Graphics::D3D12
{
	IndexBuffer::IndexBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const uint16_t> data)
	{
		m_indexBuffer = std::make_unique<Buffer<uint16_t>>(device, type, data, data.size_bytes());

		m_indexCount = static_cast<uint32_t>(data.size());

		m_indexView.BufferLocation = m_indexBuffer->Get()->GetGPUVirtualAddress();
		m_indexView.SizeInBytes = static_cast<UINT>(m_indexBuffer->GetSizeBytes());
		m_indexView.Format = DXGI_FORMAT_R16_UINT;
	}
	
	IndexBuffer::IndexBuffer(IndexBuffer && other) noexcept
	{
		m_indexBuffer = std::move(other.m_indexBuffer);
		m_indexView = std::exchange(other.m_indexView, {});
		m_indexCount = std::exchange(other.m_indexCount, 0);
	}
	
	IndexBuffer& IndexBuffer::operator=(IndexBuffer && other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_indexBuffer = std::move(other.m_indexBuffer);
		m_indexView = std::exchange(other.m_indexView, {});
		m_indexCount = std::exchange(other.m_indexCount, 0);

		return *this;
	}
}