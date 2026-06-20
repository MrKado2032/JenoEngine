#pragma once
#include "D3D12Buffer.h"
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
	struct Vertex;
	class VertexBuffer
	{
	public:
        JENO_DECLARE_MOVE_ONLY(VertexBuffer)

		VertexBuffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const Vertex> data);
		~VertexBuffer() noexcept = default;

		Buffer<Vertex>& Get() const { return *m_vertexBuffer; }
		D3D12_VERTEX_BUFFER_VIEW GetView() const { return m_vertView; }

	private:
		std::unique_ptr<Buffer<Vertex>> m_vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertView{};
	};
}