#pragma once
#include "Utils/JenoCommon.h"
#include "D3D12ConstantBuffer.h"

namespace Jeno::Graphics::D3D12
{
	struct ColorCB
	{
		DirectX::XMFLOAT4 color;
	};

	class Material
	{
	public:
		JENO_MOVE_ONLY(Material)

		explicit Material(ID3D12Device* device);
		~Material() noexcept;

		ConstantBuffer<ColorCB>& GetColorCB() const { return *m_colorCB; }

	private:
		std::unique_ptr<ConstantBuffer<ColorCB>> m_colorCB;
	};

	class Device;
	class PrimitiveMaterialFactory
	{
	public:
		static Material CreateDefault(const Device& device);
	};
}