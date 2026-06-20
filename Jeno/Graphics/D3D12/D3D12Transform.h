#pragma once
#include <DirectXMath.h>

namespace Jeno::Graphics::D3D12
{
	struct Transform
	{
		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT3 rotation{};
		DirectX::XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };

		DirectX::XMMATRIX GetWorld() const
		{
			DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
			DirectX::XMMATRIX P = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

			return S * R * P;
		}
	};
}