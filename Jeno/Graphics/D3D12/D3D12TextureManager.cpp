#include "D3D12TextureManager.h"
#include "D3D12DescriptorHeapManager.h"

#include "D3D12Device.h"
#include "D3D12CommandQueue.h"
#include "D3D12Texture.h"

#include <WICTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <d3d12.h>
#include <exception>
#include <string>

namespace Jeno::Graphics::D3D12
{
    TextureManager::TextureManager(const Device& device, DescriptorHeapManager& descriptorHeapMgr)
        : m_device(device), m_descriptorHeapMgr(descriptorHeapMgr)
    {
    }

    TextureManager::~TextureManager() noexcept
    {

    }

    TextureHandle TextureManager::LoadTexture(std::wstring_view path)
    {
        const std::wstring filePath(path);
        auto it = m_textureHandleCaches.find(filePath);
        if(it != m_textureHandleCaches.end())
        {
            return it->second;
        }

        uint32_t index;

        if(!m_freeList.empty())
        {
            index = m_freeList.back();
            m_freeList.pop_back();
        }
        else 
        {
            index = static_cast<uint32_t>(m_textures.size());
            m_textures.emplace_back();
        }

        auto& slot = m_textures[index];

        TextureHandle handle{};
        handle.index = index;
        handle.generation = slot.generation;

        TextureResource texResource{};
        texResource.descriptorHandle = m_descriptorHeapMgr.GetSRVAllocator().Allocate();
        texResource.generation = handle.generation;

        try {
            const auto device = m_device.GetNativeDevice();

            // --- Wait while the texture is loaded and the buffer is copied.

            DirectX::ResourceUploadBatch uploadBatch(device);
            uploadBatch.Begin();

            auto hr = DirectX::CreateWICTextureFromFile(device, uploadBatch, filePath.c_str(), texResource.resource.ReleaseAndGetAddressOf());
            if (FAILED(hr))
            {
                char message[256]{};
                sprintf_s(
                    message,
                    "CreateWICTextureFromFile failed. HRESULT = 0x%08X",
                    static_cast<unsigned int>(hr)
                );

                throw std::runtime_error(message);
            }

            auto uploadFinished = uploadBatch.End(m_device.GetGraphicsQueue().Get());

            uploadFinished.wait();

            // ---
            

            // --- Create the shader resouce view

            const auto desc = texResource.resource->GetDesc();

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = static_cast<UINT>(desc.MipLevels);
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.PlaneSlice = 0;
            srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

            device->CreateShaderResourceView(texResource.resource.Get(), &srvDesc, texResource.descriptorHandle.GetCPUHandle());

            // ---

            slot.resource = std::move(texResource);
            slot.alive = true;

            m_textureHandleCaches.try_emplace(filePath, handle);

            return handle;
        
        } catch (std::exception& e) {
            if (texResource.descriptorHandle.IsValid())
            {
                m_descriptorHeapMgr.GetSRVAllocator().Free(texResource.descriptorHandle);
            }
            m_freeList.push_back(index);
            throw;
        }

        return {};
    }

    void TextureManager::Free(TextureHandle& handle)
    {
        if(!handle.IsValid())
        {
            return;
        }

        auto& slot = m_textures[handle.index];
        if(!slot.alive || slot.generation != handle.generation)
        {
            handle.Reset();
            return;
        }

        m_descriptorHeapMgr.GetSRVAllocator().Free(slot.resource.descriptorHandle);

        slot.resource = {};
        slot.alive = false;
        ++slot.generation;

        m_freeList.push_back(handle.index);

        handle.Reset();
    }

    TextureResource& TextureManager::GetTextureFromHandle(const TextureHandle handle)
    {
        if (!handle.IsValid())
        {
            throw std::runtime_error("failed to get texture: invalid handle");
        }

        if (handle.index >= m_textures.size())
        {
            throw std::runtime_error("failed to get texture: index out of range");
        }

        auto& slot = m_textures[handle.index];
        if(!slot.alive)
        {
            throw std::runtime_error("failed to get texture: texture is dead");
        }

        if (slot.generation != handle.generation)
        {
            throw std::runtime_error("failed to get texture: generation mismatch");
        }

        return slot.resource;
    }
}