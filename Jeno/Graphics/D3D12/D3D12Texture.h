#pragma once
#include "D3D12Common.h"
#include "D3D12DescriptorAllocator.h"

namespace Jeno::Graphics::D3D12
{
    struct TextureHandle : public Handle
    {

    };

    struct TextureResource
    {
        uint32_t generation = 0;
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        DescriptorHandle descriptorHandle{};
    };
}