#include "D3D12Material.h"
#include "D3D12Device.h"

namespace Jeno::Graphics::D3D12
{
	Material::Material(ID3D12Device* device)
	{
		ColorCB colorCB{};
		colorCB.color = {1, 1, 0, 1};

		m_colorCB = std::make_unique<ConstantBuffer<ColorCB>>(device, colorCB);
	}

	Material::~Material() noexcept = default;

	Material PrimitiveMaterialFactory::CreateDefault(const Device& device)
	{
		return Material(device.GetNativeDevice());
	}
}