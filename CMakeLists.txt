cmake_minimum_required(VERSION 3.20)
project(JenoEngine LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

add_subdirectory(Jeno)
add_subdirectory(Sandbox)