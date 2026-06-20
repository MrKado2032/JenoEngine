#include "D3D12VertexBuffer.h"

#include "D3D12Vertex.h"

namespace Jeno::Graphics::D3D12
{
	VertexBuffer::VertexBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const Vertex> data)
	{
		m_vertexBuffer = std::make_unique<Buffer<Vertex>>(device, type, data, data.size_bytes());

		m_vertView.BufferLocation = m_vertexBuffer->Get()->GetGPUVirtualAddress();
		m_vertView.SizeInBytes = static_cast<UINT>(m_vertexBuffer->GetSizeBytes());
		m_vertView.StrideInBytes = sizeof(Vertex);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_vertView = std::exchange(other.m_vertView, {});
	}
	
	VertexBuffer& VertexBuffer::operator=(VertexBuffer && other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_vertView = std::exchange(other.m_vertView, {});

		return *this;
	}
}