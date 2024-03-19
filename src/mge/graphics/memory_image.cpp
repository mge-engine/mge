// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_image.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    memory_image::memory_image(image_format       format_,
                               const mge::extent& extent_,
                               size_t             size)
        : image(format_, extent_)
    {
        if (size == 0) {
            size = extent_.area() * mge::binary_size(format_);
            if (size == 0) {
                MGE_THROW(illegal_argument)
                    << "Size must be specified for format " << format_;
            }
        }
        m_data.resize(size);
    }

    void* memory_image::data() const
    {
        return const_cast<mge::buffer::value_type*>(m_data.data());
    }

    std::span<uint8_t> memory_image::data_span() const
    {
        return std::span<uint8_t>(const_cast<mge::buffer&>(m_data));
    }

    size_t memory_image::binary_size() const { return m_data.size(); }

} // namespace mge