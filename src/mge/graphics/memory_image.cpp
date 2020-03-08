// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_image.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    memory_image::memory_image(const mge::extent& e,
                               const image_format& f,
                               bool mapped)
        : image(e, f)
        , m_data(nullptr)
        , m_initially_mapped(mapped)
    {
        if(mapped) {
            map();
        }
    }

    memory_image::memory_image(const mge::extent& e,
                               const image_format& f,
                               void *data,
                               bool take_ownership)
        : image(e, f)
        , m_data(nullptr)
        , m_initially_mapped(false)
    {
        if (!data) {
            MGE_THROW(illegal_argument) << "Data for memory image must not be null";
        }

        if (take_ownership) {
            m_data = reinterpret_cast<uint8_t *>(data);
            m_initially_mapped = true;
            m_map_counter++;
        } else {
            memcpy(map(), data, buffer_size());
            m_initially_mapped = true;
        }
    }

    memory_image::~memory_image()
    {
        unmap();
    }

    void *
    memory_image::on_map()
    {
        if(!m_data) {
            m_data = reinterpret_cast<uint8_t *>(mge::malloc(buffer_size()));
        }
        return m_data;
    }

    void
    memory_image::on_unmap()
    {
        if(m_data) {
            mge::free(m_data);
        }
    }


}
