# Jenoを静的ライブラリとして扱う
add_library(Jeno STATIC 
    Jeno.cpp 
    Core/Application.cpp 
    Core/Window.cpp 
    Core/ShaderCompiler.cpp 
    Graphics/D3D12/D3D12Renderer.cpp
    Graphics/D3D12/D3D12CommandContext.cpp
    Graphics/D3D12/D3D12CommandListManager.cpp
    Graphics/D3D12/D3D12CommandQueue.cpp
    Graphics/D3D12/D3D12DescriptorAllocator.cpp
    Graphics/D3D12/D3D12DescriptorHeap.cpp
    Graphics/D3D12/D3D12DescriptorHeapManager.cpp
    Graphics/D3D12/D3D12Device.cpp
    Graphics/D3D12/D3D12Swapchain.cpp
    Graphics/D3D12/D3D12Helper.cpp
    Graphics/D3D12/D3D12VertexBuffer.cpp
    Graphics/D3D12/D3D12IndexBuffer.cpp
    Graphics/D3D12/D3D12Mesh.cpp
    Graphics/D3D12/D3D12Camera.cpp
    Graphics/D3D12/D3D12Material.cpp
    )

# JenoはC++20を使う
target_compile_features(Jeno PUBLIC cxx_std_20)

# Jeno直下ディレクトリを外部にインクルードする
target_include_directories(Jeno PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# PrecompiledHeader 
target_precompile_headers(Jeno PRIVATE PrecompiledHeader.h)

target_link_libraries(Jeno PUBLIC glfw d3d12 dxgi)