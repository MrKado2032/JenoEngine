#pragma once
#include "D3D12Helper.h"
#include "Utils/JenoCommon.h"
#include "D3D12Common.h"

namespace Jeno::Graphics::D3D12
{
	template<typename T>
	class Buffer
	{
	public:
		JENO_DECLARE_MOVE_ONLY(Buffer)

		Buffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const T> data, size_t data_size, bool unmap = true);
		virtual ~Buffer() noexcept = default;

		void Map(std::span<const T> data, bool unmap = true);
		void Unmap();
		
		ID3D12Resource* Get() const { return m_buffer.Get(); }
		size_t GetSizeBytes() const { return m_bufferSize; }

	protected:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
		size_t m_bufferSize = 0;
		D3D12_HEAP_TYPE m_heapType = D3D12_HEAP_TYPE_UPLOAD;
	};

	template<typename T>
	inline Buffer<T>::Buffer(ID3D12Device* device, D3D12_HEAP_TYPE type, std::span<const T> data, size_t data_size, bool unmap)
	{
		m_bufferSize = data_size;
		m_heapType = type;

		Helper::CreateBuffer(device, type, m_bufferSize, m_buffer);

		if (!data.empty() && type == D3D12_HEAP_TYPE_UPLOAD)
		{
			Map(data, unmap);
		}
	}

	template<typename T>
	inline Buffer<T>::Buffer(Buffer&& other) noexcept
	{
		m_buffer = std::move(other.m_buffer);
		m_bufferSize = std::exchange(other.m_bufferSize, 0);
		m_heapType = std::exchange(other.m_heapType, D3D12_HEAP_TYPE_UPLOAD);
	}

	template<typename T>
	inline Buffer<T>& Buffer<T>::operator=(Buffer<T>&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		m_buffer = std::move(other.m_buffer);
		m_bufferSize = std::exchange(other.m_bufferSize, 0);
		m_heapType = std::exchange(other.m_heapType, D3D12_HEAP_TYPE_UPLOAD);

		return *this;
	}

	template<typename T>
	inline void Buffer<T>::Map(std::span<const T> data, bool unmap)
	{
		if (!m_buffer)
		{
			throw std::runtime_error("buffer is null");
		}

		if (m_heapType != D3D12_HEAP_TYPE_UPLOAD)
		{
			throw std::runtime_error("only upload heap buffer can be mapped directly");
		}

		const size_t uploadSize = data.size_bytes();

		if (uploadSize > m_bufferSize)
		{
			throw std::runtime_error("buffer upload data is larger than buffer size");
		}

		const D3D12_RANGE readRange{ 0, 0 };
		
		void* mapped = nullptr;
		JENO_THROW_IF_FAILED(m_buffer->Map(0, &readRange, &mapped));
		std::memset(mapped, 0, m_bufferSize);
		std::memcpy(mapped, data.data(), uploadSize);
		if(unmap) m_buffer->Unmap(0, nullptr);
	}

	template<typename T>
	inline void Buffer<T>::Unmap()
	{
		m_buffer->Unmap(0, nullptr);
	}
}