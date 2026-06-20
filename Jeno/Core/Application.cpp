#include "Application.h"
#include "Window.h"

namespace Jeno::Core 
{
    Application::Application()
    {
        m_window = std::make_unique<Window>(1280, 720, "Jeno Engine");
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
        }
    }
}