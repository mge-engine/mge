// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace mge {
    using buffer = std::vector<std::byte>;
    using buffer_ref = std::shared_ptr<buffer>;

    inline buffer_ref make_buffer(size_t size)
    {
        return std::make_shared<buffer>(size);
    }

    inline buffer_ref make_buffer(const void* data, size_t size)
    {
        auto buf = std::make_shared<buffer>(size);
        if (data && size > 0) {
            std::memcpy(buf->data(), data, size);
        }
        return buf;
    }

    inline buffer_ref make_buffer(const std::byte* begin, const std::byte* end)
    {
        return std::make_shared<buffer>(begin, end);
    }

    template <typename T, size_t N>
    inline buffer_ref make_buffer(const T (&arr)[N])
    {
        return make_buffer(reinterpret_cast<const std::byte*>(arr),
                           sizeof(T) * N);
    }

    template <typename T, size_t N>
    inline buffer_ref make_buffer(const std::array<T, N>& arr)
    {
        return make_buffer(reinterpret_cast<const std::byte*>(arr.data()),
                           sizeof(T) * N);
    }

} // namespace mge