#pragma once

namespace Jeno::Core
{
	class Window;
}

namespace Jeno::Graphics::D3D12
{
	struct Transform;

	class Device;
	class DescriptorHeapManager;
	class CommandListManager;
	class Swapchain;
	class Mesh;
	class Material;
	class Camera;
	class Renderer
	{
	public:
		explicit Renderer(Core::Window& window);
		~Renderer() noexcept;

		void BeginFrame();
		void EndFrame();
		// void DrawMesh(const Mesh& mesh, const Material& material, const Transform& transform);
		void Resize(uint32_t width, uint32_t height);

		Device& GetDevice() const noexcept { return *m_device; }

	private:
		struct FrameContext
		{
			uint64_t fenceValue = 0;
		};

		std::unique_ptr<Device> m_device;
		std::unique_ptr<DescriptorHeapManager> m_descriptorHeapManager;
		std::unique_ptr<CommandListManager> m_commandListManager;
		std::unique_ptr<Swapchain> m_swapchain;

		std::vector<FrameContext> m_frames;

		// Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		// Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

		// std::unique_ptr<Camera> m_camera;
	};
}