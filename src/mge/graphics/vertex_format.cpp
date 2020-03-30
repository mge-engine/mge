// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_format.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    std::ostream &operator<<(std::ostream &os, const vertex_format &fmt)
    {
        return os << "vertex_format[type=" << fmt.type()
                  << ", size=" << (uint32_t)fmt.size() << "]";
    }

    size_t vertex_format::binary_size() const
    {
        size_t result = data_type_size(m_type);
        result *= m_size;
        return result;
    }
} // namespace mge