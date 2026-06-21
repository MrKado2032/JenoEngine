#include "Application.h"
#include "Window.h"

#include "Graphics/D3D12/D3D12Renderer.h"

#include "Graphics/D3D12/RenderObject.h"
#include "Graphics/D3D12/D3D12Device.h"
#include "Graphics/D3D12/D3D12TextureManager.h"

namespace Jeno::Core 
{
    Application::Application()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

        if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
        {
            throw;
        }

        m_window = std::make_unique<Window>(1280, 720, "Jeno Engine");
        m_renderer = std::make_unique<Graphics::D3D12::Renderer>(*m_window);
    }

    Application::~Application() noexcept
    {
        m_renderer.reset();
		m_renderObjects.clear();
		m_window.reset();

        CoUninitialize();
    }

    void Application::Run()
    {
        Start();
        while(m_window->IsRunning())
        {
            m_window->UpdateEvents();
            if(m_window->IsResize())
            {
                const uint32_t width = m_window->GetWidth();
                const uint32_t height = m_window->GetHeight();

                if (width > 0 && height > 0)
                {
                    m_renderer->Resize(width, height);
                }

                m_window->ResetResizeFlag();
            }

            m_renderer->BeginFrame();
            Update(1);
            for (auto& obj : m_renderObjects)
			{
				m_renderer->DrawMesh(obj.mesh, obj.material, obj.transform, *obj.transformCB);
			}
            m_renderer->EndFrame();
        }
    }

    void Application::CreateQuad(float width, float height, const Graphics::D3D12::Transform& transform)
	{
		Graphics::D3D12::Mesh addMesh(m_renderer->GetDevice(), Graphics::D3D12::PrimitiveGeometryFactory::CreateQuad(width, height, { 0.5f, 0.5f }));
		Graphics::D3D12::Material addMaterial(Graphics::D3D12::PrimitiveMaterialFactory::CreateDefault(m_renderer->GetDevice()));

        addMaterial.AttachTextureHandle(m_renderer->GetTextureManager().LoadTexture(L"Assets/textures/test.png"));

        Graphics::D3D12::TransformCB transformCB{};

		Graphics::D3D12::RenderObject renObj
		{
			.mesh = std::move(addMesh),
			.material = std::move(addMaterial),
			.transform = transform,
            .transformCB = 
                std::make_unique<Graphics::D3D12::ConstantBuffer<Graphics::D3D12::TransformCB>>(m_renderer->GetDevice().GetNativeDevice(), transformCB)
		};

		m_renderObjects.push_back(std::move(renObj));
	}
}