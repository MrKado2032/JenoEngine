#pragma once
#include <DirectXMath.h>

namespace Jeno::Graphics::D3D12
{
    struct TransformCB
    {
        DirectX::XMFLOAT4X4 mvp;
    };
}