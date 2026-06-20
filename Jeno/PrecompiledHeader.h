#pragma once

#ifdef _WIN32
#include <d3d12.h>
// #include <d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <optional>
#include <span>
#include <memory>
#include <stdexcept>
#include <utility>