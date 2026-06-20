#include "D3D12Material.h"

#include "D3D12Device.h"

namespace Jeno::Graphics::D3D12
{
	Material::Material(ID3D12Device* device)
	{
		TransformCB transformCB{};

		m_transformCB = std::make_unique<ConstantBuffer<TransformCB>>(device, transformCB);
	}

	Material::~Material() noexcept = default;

	Material PrimitiveMaterialFactory::CreateDefault(const Device& device)
	{
		return Material(device.GetNativeDevice());
	}
}