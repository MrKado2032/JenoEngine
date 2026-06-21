#include "D3D12Material.h"
#include "D3D12Device.h"
#include "D3D12ShaderTypes.h"

namespace Jeno::Graphics::D3D12
{
	Material::Material(ID3D12Device* device)
	{
		ColorCB colorCB{};
		colorCB.color = {1, 1, 1, 1};

		m_colorCB = std::make_unique<ConstantBuffer<ColorCB>>(device, colorCB);
	}

	Material::~Material() noexcept = default;

	void Material::AttachTextureHandle(TextureHandle handle)
	{
		m_texHandle = handle;
	}

	Material PrimitiveMaterialFactory::CreateDefault(const Device& device)
	{
		return Material(device.GetNativeDevice());
	}
}