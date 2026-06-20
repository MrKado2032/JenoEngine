#pragma once
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
	class Camera
	{
	public:
		JENO_DECLARE_MOVE_ONLY(Camera)

		Camera(uint32_t width, uint32_t height, float minDepth = -1.0f, float maxDepth = 100.0f);
		~Camera() noexcept = default;

		const DirectX::XMMATRIX GetViewProjMatrix() { return m_view * m_proj; }
		const uint32_t GetWidth() const { return m_width; }
		const uint32_t GetHeight() const { return m_height; }

	private:
		DirectX::XMMATRIX m_view{};
		DirectX::XMMATRIX m_proj{};

		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
}