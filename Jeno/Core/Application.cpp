#include "Application.h"
#include "Window.h"

#include "Graphics/D3D12/D3D12Renderer.h"

namespace Jeno::Core 
{
    Application::Application()
    {
        m_window = std::make_unique<Window>(1280, 720, "Jeno Engine");
        m_renderer = std::make_unique<Graphics::D3D12::Renderer>(*m_window);
    }

    Application::~Application() noexcept
    {

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
            m_renderer->EndFrame();
        }
    }
}