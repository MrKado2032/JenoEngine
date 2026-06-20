#pragma once
#include "D3D12Vertex.h"

namespace Jeno::Graphics::D3D12
{
	struct Geometry
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};

	class PrimitiveGeometryFactory
	{
	public:
		static Geometry CreateQuad(float width, float height, DirectX::XMFLOAT2 pivot = { 0.0f, 0.0f });
	};

	class VertexBuffer;
	class IndexBuffer;
	class Device;
	class Mesh
	{
	public:
		Mesh(Device& device, const Geometry& geometry);
		~Mesh() noexcept;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;

		VertexBuffer& GetVertexBuffer() noexcept
		{
			return *m_vertexBuffer;
		}

		const VertexBuffer& GetVertexBuffer() const noexcept
		{
			return *m_vertexBuffer;
		}

		IndexBuffer& GetIndexBuffer() noexcept
		{
			return *m_indexBuffer;
		}

		const IndexBuffer& GetIndexBuffer() const noexcept
		{
			return *m_indexBuffer;
		}

		D3D12_VERTEX_BUFFER_VIEW GetVertView() const noexcept;

		D3D12_INDEX_BUFFER_VIEW GetIndexView() const noexcept;

		uint32_t GetIndexCount() const noexcept;

	private:
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
	};
}