#include "D3D12Camera.h"

namespace Jeno::Graphics::D3D12
{
	Camera::Camera(uint32_t width, uint32_t height, float minDepth, float maxDepth) : m_width(width), m_height(height)
	{
		m_view = DirectX::XMMatrixIdentity();
		m_proj = DirectX::XMMatrixOrthographicOffCenterLH(0, static_cast<float>(width), static_cast<float>(height), 0, minDepth, maxDepth);
	}

	Camera::Camera(Camera&& other) noexcept
	{
		m_view = std::exchange(other.m_view, {});
		m_proj = std::exchange(other.m_proj, {});
	}

	Camera& Camera::operator=(Camera&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_view = std::exchange(other.m_view, {});
		m_proj = std::exchange(other.m_proj, {});

		return *this;
	}
}