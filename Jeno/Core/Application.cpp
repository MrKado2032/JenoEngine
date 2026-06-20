#include "Application.h"
#include "Window.h"

#include "Graphics/D3D12/D3D12Renderer.h"

#include "Graphics/D3D12/RenderObject.h"

namespace Jeno::Core 
{
    Application::Application()
    {
        m_window = std::make_unique<Window>(1280, 720, "Jeno Engine");
        m_renderer = std::make_unique<Graphics::D3D12::Renderer>(*m_window);
    }

    Application::~Application() noexcept
    {
		m_renderObjects.clear();
		m_renderer.reset();
		m_window.reset();
    }

    void Application::Run()
    {
        Start();
        while(m_window->IsRunning())
        {
            m_window->UpdateEvents();
            if(m_window->IsResize())
            {
                m_window->ResetResizeFlag();
            }

            m_renderer->BeginFrame();
            Update(1);
            for (uint32_t i = 0; auto& obj : m_renderObjects)
			{
				m_renderer->DrawMesh(obj.mesh, obj.material, obj.transform);
			}
            m_renderer->EndFrame();
        }
    }

    void Application::CreateQuad(float width, float height, const Graphics::D3D12::Transform& transform)
	{
		Graphics::D3D12::Mesh addMesh(m_renderer->GetDevice(), Graphics::D3D12::PrimitiveGeometryFactory::CreateQuad(width, height, { 0.5f, 0.5f }));
		Graphics::D3D12::Material addMaterial(Graphics::D3D12::PrimitiveMaterialFactory::CreateDefault(m_renderer->GetDevice()));

		Graphics::D3D12::RenderObject renObj
		{
			.mesh = std::move(addMesh),
			.material = std::move(addMaterial),
			.transform = transform
		};

		m_renderObjects.push_back(std::move(renObj));
	}
}