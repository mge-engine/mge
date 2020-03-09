// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image.hpp"
#include "mge/graphics/memory_image.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    image::image(const mge::extent& e,
                 const image_format& format)
        :m_extent(e)
        ,m_format(format)
        ,m_map_ptr(nullptr)
        ,m_map_counter(0)
    {}

    void *
    image::map()
    {
        if (++m_map_counter == 1) {
            m_map_ptr = on_map();
        }
        return m_map_ptr;
    }

    void
    image::unmap()
    {
        if (--m_map_counter == 0) {
            m_map_ptr =  nullptr;
            on_unmap();
        }
    }

    void *
    image::scanline(uint32_t line) const
    {
        if(line >= extent().height()) {
            MGE_THROW(illegal_argument) << "Invalid scan line " << line;
        }
        const uint8_t *ptr = static_cast<const uint8_t *>(m_map_ptr);
        if (!ptr) {
            MGE_THROW(illegal_state) << "Image is not mapped";
        }
        return const_cast<uint8_t *>(ptr + line * extent().width() * pixel_size(format()));
    }

    image_ref
    image::clone()
    {
        image_ref result = std::make_shared<memory_image>(extent(), format(), map());
        unmap();
        return result;
    }

    size_t
    image::buffer_size() const
    {
        return extent().area() * pixel_size(format());
    }


    rgba_color
    image::pixel_color(uint32_t x, uint32_t y) const
    {
        if (x > extent().width()) {
            MGE_THROW(illegal_argument) << "Invalid x position " << x;
        }
        uint8_t *sl = static_cast<uint8_t *>(scanline(y));
        uint8_t *pl = sl + x * pixel_size(format());
        return rgba_color(pl, format());
    }
}
