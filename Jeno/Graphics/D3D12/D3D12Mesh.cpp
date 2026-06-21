#include "D3D12Mesh.h"

#include "D3D12Device.h"
#include "D3D12VertexBuffer.h"
#include "D3D12IndexBuffer.h"

namespace Jeno::Graphics::D3D12
{
	Geometry PrimitiveGeometryFactory::CreateQuad(float width, float height, DirectX::XMFLOAT2 pivot)
	{
		const float left = -width * pivot.x;
		const float right = width * (1.0f - pivot.x);
		const float top = -height * pivot.y;
		const float bottom = height * (1.0f - pivot.y);

		Geometry geometry{};

		geometry.vertices =
		{
			{{left,  top,    0.0f}, {0, 0}},
			{{right, top,    0.0f}, {1, 0}},
			{{right, bottom, 0.0f}, {1, 1}},
			{{left,  bottom, 0.0f}, {0, 1}},
		};

		geometry.indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		return geometry;
	}

	Mesh::Mesh(Device& device, const Geometry& geometry)
	{
		m_vertexBuffer = std::make_unique<VertexBuffer>(device.GetNativeDevice(), D3D12_HEAP_TYPE_UPLOAD, geometry.vertices);
		m_indexBuffer = std::make_unique<IndexBuffer>(device.GetNativeDevice(), D3D12_HEAP_TYPE_UPLOAD, geometry.indices);
	}

	Mesh::~Mesh() noexcept = default;

	Mesh::Mesh(Mesh&& other) noexcept
	{
		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_indexBuffer = std::move(other.m_indexBuffer);
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_indexBuffer = std::move(other.m_indexBuffer);

		return *this;
	}

	D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertView() const noexcept
	{
		return m_vertexBuffer->GetView();
	}

	D3D12_INDEX_BUFFER_VIEW Mesh::GetIndexView() const noexcept
	{
		return m_indexBuffer->GetView();
	}

	uint32_t Mesh::GetIndexCount() const noexcept
	{
		return m_indexBuffer->GetIndexCount();
	}
}