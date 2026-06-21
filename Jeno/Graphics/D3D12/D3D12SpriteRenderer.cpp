#include "D3D12SpriteRenderer.h"

#include "D3D12Renderer.h"
#include "D3D12Mesh.h"

namespace Jeno::Graphics::D3D12
{
    SpriteRenderer::SpriteRenderer(Renderer& renderer) : m_renderer(renderer)
    {
        m_quadMesh = std::make_unique<Mesh>(renderer.GetDevice(), PrimitiveGeometryFactory::CreateQuad(1, 1));
    }

    void SpriteRenderer::Draw(const Material& material, const Transform& transform, ConstantBuffer<TransformCB>& consBuffer)
    {
        m_renderer.DrawMesh(*m_quadMesh, material, transform, consBuffer);
    }
}