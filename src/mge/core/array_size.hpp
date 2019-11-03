#pragma once
#include "mge/core/types.hpp"

namespace mge {

    template<typename T, size_t size>
    constexpr size_t array_size(T(&)[size])
    {
        return size;
    }

}