#pragma once
#include "D3D12ConstantBuffer.h"
#include "D3D12ShaderTypes.h"
#include "D3D12Texture.h"

namespace Jeno::Core
{
	class Window;
}

namespace Jeno::Graphics::D3D12
{
	struct Transform;
    struct TransformCB;

	class Device;
	class DescriptorHeapManager;
	class CommandListManager;
	class TextureManager;
	class Swapchain;
	class Mesh;
	class Material;
	class Camera;
	class PipelineLayout;
	class Pipeline;
	class Renderer
	{
	public:
		explicit Renderer(Core::Window& window);
		~Renderer() noexcept;

		void BeginFrame();
		void EndFrame();
		void DrawMesh(const Mesh& mesh, const Material& material, const Transform& transform, ConstantBuffer<TransformCB>& consBuffer);
		void Resize(uint32_t width, uint32_t height);

		Device& GetDevice() const noexcept { return *m_device; }
		TextureManager& GetTextureManager() const noexcept { return *m_textureManager; }

	private:
		struct FrameContext
		{
			uint64_t fenceValue = 0;
		};

		std::unique_ptr<Device> m_device;
		std::unique_ptr<DescriptorHeapManager> m_descriptorHeapManager;
		std::unique_ptr<CommandListManager> m_commandListManager;
		std::unique_ptr<TextureManager> m_textureManager;
		std::unique_ptr<Swapchain> m_swapchain;

		std::vector<FrameContext> m_frames;

		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<PipelineLayout> m_pipelineLayout;
		std::unique_ptr<Pipeline> m_pipeline;
		
		TextureHandle m_defaultTexHandle{};
	};
}