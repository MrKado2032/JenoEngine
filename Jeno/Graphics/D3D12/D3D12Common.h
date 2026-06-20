#pragma once
#include <sstream>

namespace Jeno::Graphics::D3D12 
{
    inline void ThrowIfFailedImpl(HRESULT hr, const char* file, int line)
	{
        if (FAILED(hr))
        {
            std::ostringstream oss;
            oss << "HRESULT ERROR: 0x"
                << std::hex << std::uppercase
                << static_cast<unsigned long>(hr)
                << " at " << file << ":" << std::dec << line;

            throw std::runtime_error(oss.str());
        }
	}

    struct Handle
    {
        static constexpr uint32_t kInvalidValue = UINT32_MAX;

        Handle() noexcept = default;
        ~Handle() noexcept = default;

        Handle(const Handle&) noexcept = default;
        Handle& operator=(const Handle&) noexcept = default;

        Handle(Handle&& other) noexcept
        {
            index = std::exchange(other.index, kInvalidValue);
            generation = std::exchange(other.generation, 0);
        }

        Handle& operator=(Handle&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            index = std::exchange(other.index, kInvalidValue);
            generation = std::exchange(other.generation, 0);

            return *this;
        }

        uint32_t index = kInvalidValue;
        uint32_t generation = 0;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return index != kInvalidValue;
        }

        void Reset() noexcept
        {
            index = kInvalidValue;
            generation = 0;
        }
    };
}

#define JENO_THROW_IF_FAILED(hr) Jeno::Graphics::D3D12::ThrowIfFailedImpl((hr), __FILE__, __LINE__)