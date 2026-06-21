#pragma once
#include <DirectXMath.h>

namespace Jeno::Graphics::D3D12
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
}