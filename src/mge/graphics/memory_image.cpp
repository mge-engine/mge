// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_image.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    memory_image::memory_image(image_format       format_,
                               const mge::extent& extent_,
                               size_t             size)
        : image(format_, extent_)
        , m_data(nullptr)
        , m_data_size(0)
    {
        if (size == 0) {
            size = extent_.area() * mge::binary_size(format_);
            if (size == 0) {
                MGE_THROW(illegal_argument)
                    << "Size must be specified for format " << format_;
            }
        }
        m_data = mge::malloc(size);
        m_data_size = size;
    }

    memory_image::memory_image(image_format       format_,
                               const mge::extent& extent_,
                               void*              data_,
                               bool               copy_data,
                               size_t             size)
        : image(format_, extent_)
        , m_data(copy_data ? nullptr : data_)
        , m_data_size(0)
    {
        if (size == 0) {
            size = extent_.area() * mge::binary_size(format_);
            if (size == 0) {
                MGE_THROW(illegal_argument)
                    << "Size must be specified for format " << format_;
            }
        }
        if (copy_data) {
            m_data = mge::malloc(size);
            std::memcpy(m_data, data_, size);
        }
        m_data_size = size;
    }

    memory_image::~memory_image() { mge::free(m_data); }

    void* memory_image::data() const { return m_data; }

    std::span<uint8_t> memory_image::data_span() const
    {
        return std::span<uint8_t>(static_cast<uint8_t*>(m_data), m_data_size);
    }

    size_t memory_image::binary_size() const { return m_data_size; }

} // namespace mge