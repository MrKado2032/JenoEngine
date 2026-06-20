#pragma once
#include "D3D12Buffer.h"
#include "Utils/JenoCommon.h"

namespace Jeno::Graphics::D3D12
{
	template<typename T>
	class ConstantBuffer
	{
	public:
		JENO_DEFAULT_MOVABLE(ConstantBuffer)

		ConstantBuffer(ID3D12Device* device, const T& data);
		~ConstantBuffer() noexcept;

		D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress() const { return m_buffer->Get()->GetGPUVirtualAddress(); }

		void Update(const T& data);

	private:
		std::unique_ptr<Buffer<T>> m_buffer;
		
	};

	template<typename T>
	inline ConstantBuffer<T>::ConstantBuffer(ID3D12Device* device, const T& data)
	{
		if (!device)
		{
			throw std::runtime_error("constant buffer device is null");
		}

		const std::span<const T> dataSpan{ &data, 1 };
		const auto alignedSize = (sizeof(data) + D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
		m_buffer = std::make_unique<Buffer<T>>(device, D3D12_HEAP_TYPE_UPLOAD, dataSpan, alignedSize, false);
	}

	template<typename T>
	inline ConstantBuffer<T>::~ConstantBuffer() noexcept
	{
		m_buffer->Unmap();
		m_buffer.reset();
	}

	template<typename T>
	inline void ConstantBuffer<T>::Update(const T& data)
	{
		const std::span<const T> dataSpan{ &data, 1 };
		m_buffer->Map(dataSpan, false);
	}
}