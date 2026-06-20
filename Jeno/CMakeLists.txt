# Jenoを静的ライブラリとして扱う
add_library(Jeno STATIC Jeno.cpp Core/Application.cpp Core/Window.cpp)

# JenoはC++20を使う
target_compile_features(Jeno PUBLIC cxx_std_20)

# Jeno直下ディレクトリを外部にインクルードする
target_include_directories(Jeno PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# PrecompiledHeader 
target_precompile_headers(Jeno PRIVATE PrecompiledHeader.h)

target_link_libraries(Jeno PUBLIC glfw)