#pragma once
#include "Graphics/D3D12/D3D12Mesh.h"
#include "Graphics/D3D12/D3D12Material.h"
#include "Graphics/D3D12/D3D12Transform.h"

namespace Jeno::Graphics::D3D12
{
	struct RenderObject
	{
		Graphics::D3D12::Mesh mesh;
		Graphics::D3D12::Material material;
		Graphics::D3D12::Transform transform;
	};
}