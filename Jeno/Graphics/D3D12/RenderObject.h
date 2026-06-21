#pragma once
#include "Graphics/D3D12/D3D12Material.h"
#include "Graphics/D3D12/D3D12Transform.h"
#include "Graphics/D3D12/D3D12ShaderTypes.h"

namespace Jeno::Graphics::D3D12
{
	struct RenderObject
	{
		Graphics::D3D12::Material material;
		Graphics::D3D12::Transform transform;

		std::unique_ptr<ConstantBuffer<TransformCB>> transformCB;
	};
}