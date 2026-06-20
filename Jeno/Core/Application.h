#pragma once
#include <memory>

namespace Jeno::Graphics::D3D12
{
	class Renderer;
}

namespace Jeno::Core
{
    class Window;
	class Application
	{
	public:
		Application();
		virtual ~Application() noexcept;

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		void Run();

	protected:
		virtual void Start() = 0;
		virtual void Update(float dt) = 0;

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Graphics::D3D12::Renderer> m_renderer;
	};
}