#pragma once

namespace Jeno::Core 
{
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, const char* title);
		~Window() noexcept;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;

		[[nodiscard]] bool IsRunning() const;
		void UpdateEvents();

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		HWND GetWindowHandle() const;
		bool IsResize() const { return m_isResize; }
		void ResetResizeFlag();

	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		GLFWwindow* m_glfwWindow = nullptr;
		bool m_isResize = false;
	};
}