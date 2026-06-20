#pragma once
#include "Utils/JenoCommon.h"
#include "D3D12ConstantBuffer.h"

namespace Jeno::Graphics::D3D12
{
	struct TransformCB
	{
		DirectX::XMFLOAT4X4 mvp;
	};

	class Material
	{
	public:
		JENO_MOVE_ONLY(Material)

		explicit Material(ID3D12Device* device);
		~Material() noexcept;

		ConstantBuffer<TransformCB>& GetTransformCB() const { return *m_transformCB; }

	private:
		std::unique_ptr<ConstantBuffer<TransformCB>> m_transformCB;
	};

	class Device;
	class PrimitiveMaterialFactory
	{
	public:
		static Material CreateDefault(const Device& device);
	};
}