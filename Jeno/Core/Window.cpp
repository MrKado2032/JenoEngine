#include "Window.h"

namespace Jeno::Core
{
	Window::Window(uint32_t width, uint32_t height, const char* title)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("failed to init glfw");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_glfwWindow = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
		if (!m_glfwWindow)
		{
			glfwTerminate();
			throw std::runtime_error("failed to create glfw window");
		}

		glfwSetWindowUserPointer(m_glfwWindow, this);
		glfwSetFramebufferSizeCallback(m_glfwWindow, framebuffer_size_callback);
	}

	Window::~Window() noexcept
	{
		if (m_glfwWindow)
		{
			glfwDestroyWindow(m_glfwWindow);
			m_glfwWindow = nullptr;

			glfwTerminate();
		}
	}

	Window::Window(Window&& other) noexcept
		: m_glfwWindow(std::exchange(other.m_glfwWindow, nullptr))
	{
	}

	Window& Window::operator=(Window&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		if (m_glfwWindow)
		{
			glfwDestroyWindow(m_glfwWindow);
		}

		m_glfwWindow = std::exchange(other.m_glfwWindow, nullptr);

		return *this;
	}

	bool Window::IsRunning() const
	{
		return !glfwWindowShouldClose(m_glfwWindow);
	}

	void Window::UpdateEvents()
	{
		glfwPollEvents();
	}

	uint32_t Window::GetWidth() const
	{
		int width;
		glfwGetFramebufferSize(m_glfwWindow, &width, nullptr);

		return static_cast<uint32_t>(width);
	}

	uint32_t Window::GetHeight() const
	{
		int height;
		glfwGetFramebufferSize(m_glfwWindow, nullptr, &height);

		return static_cast<uint32_t>(height);
	}

	HWND Window::GetWindowHandle() const
	{
		return glfwGetWin32Window(m_glfwWindow);
	}

	void Window::ResetResizeFlag()
	{
		m_isResize = false;
	}

	void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		auto ptr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (ptr)
		{
			ptr->m_isResize = true;
		}
	}
}