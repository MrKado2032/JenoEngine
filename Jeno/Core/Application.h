#pragma once
#include <memory>
#include <vector>

namespace Jeno::Graphics::D3D12
{
	class Renderer;
	struct RenderObject;
	struct Transform;
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

		void CreateQuad(float width, float height, const Graphics::D3D12::Transform& transform);

	private:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Graphics::D3D12::Renderer> m_renderer;

		std::vector<Graphics::D3D12::RenderObject> m_renderObjects;
	};
}