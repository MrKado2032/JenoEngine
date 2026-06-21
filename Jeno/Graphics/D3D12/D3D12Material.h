#pragma once
#include "Utils/JenoCommon.h"
#include "D3D12ConstantBuffer.h"
#include "D3D12Texture.h"

namespace Jeno::Graphics::D3D12
{
	struct ColorCB;
	class Material
	{
	public:
		JENO_MOVE_ONLY(Material)

		explicit Material(ID3D12Device* device);
		~Material() noexcept;

		void AttachTextureHandle(TextureHandle handle);

		ConstantBuffer<ColorCB>& GetColorCB() const { return *m_colorCB; }
		TextureHandle GetTextureHandle() const { return m_texHandle; }

	private:
		std::unique_ptr<ConstantBuffer<ColorCB>> m_colorCB;
		TextureHandle m_texHandle{};
	};

	class Device;
	class PrimitiveMaterialFactory
	{
	public:
		static Material CreateDefault(const Device& device);
	};
}