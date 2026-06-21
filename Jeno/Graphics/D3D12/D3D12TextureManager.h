#pragma once
#include "Utils/JenoCommon.h"
#include <string_view>
#include "D3D12Texture.h"

namespace Jeno::Graphics::D3D12
{
    struct TextureSlot
    {
        TextureResource resource{};
        uint32_t generation = 0;
        bool alive = false;
    };

    class DescriptorHeapManager;
    class Device;
    class TextureManager
    {
    public:
        JENO_NON_MOVE_AND_COPYABLE(TextureManager)

        TextureManager(const Device& device, DescriptorHeapManager& descriptorHeapMgr);
        ~TextureManager() noexcept;

        TextureHandle LoadTexture(std::wstring_view path);
        void Free(TextureHandle& handle);

        TextureResource& GetTextureFromHandle(const TextureHandle handle);

    private:
        DescriptorHeapManager& m_descriptorHeapMgr;
        const Device& m_device;

        std::vector<TextureSlot> m_textures;
        std::vector<uint32_t> m_freeList;
    };
}