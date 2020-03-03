// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_image.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    memory_image::memory_image(const mge::extent& e, const image_format& f)
        :image(e, f)
    {
        if(m_data.size() != buffer_size()) {
            m_data.resize(buffer_size());
        }
    }

    memory_image::memory_image(const mge::extent& e, const image_format& f, void *data)
        :image(e, f)
    {
        if (!data) {
            MGE_THROW(illegal_argument) << "Data for memory image must not be null";
        }

        memcpy(map(), data, buffer_size());
        unmap();
    }

    memory_image::~memory_image()
    {}

    void *
    memory_image::on_map()
    {
        if(m_data.size() != buffer_size()) {
            m_data.resize(buffer_size());
        }
        return m_data.data();
    }

    void
    memory_image::on_unmap()
    {}

    void *
    memory_image::scanline(uint32_t line)
    {
        if(line >= extent().height()) {
            MGE_THROW(illegal_argument) << "Invalid scan line " << line;
        }
        uint8_t *ptr = m_data.data();
        return ptr + line * extent().width() * pixel_size(format());
    }

    size_t
    memory_image::buffer_size() const
    {
        return extent().area() * pixel_size(format());
    }
}
