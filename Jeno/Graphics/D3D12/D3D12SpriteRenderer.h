#pragma once
#include "Utils/JenoCommon.h"
#include "D3D12ConstantBuffer.h"

namespace Jeno::Graphics::D3D12
{
    class Material;
    struct Transform;
    struct TransformCB;
    class Mesh;
    class Renderer;
    class SpriteRenderer
    {
    public:
        JENO_NON_MOVE_AND_COPYABLE(SpriteRenderer)
        SpriteRenderer(Renderer& renderer);
        ~SpriteRenderer() noexcept = default;

        void Draw(const Material& material, const Transform& transform, ConstantBuffer<TransformCB>& consBuffer);

    private:
        std::unique_ptr<Mesh> m_quadMesh;
        Renderer& m_renderer;
    };
}