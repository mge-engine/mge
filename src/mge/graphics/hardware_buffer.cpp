// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/hardware_buffer.hpp"

namespace mge {

    hardware_buffer::hardware_buffer(render_context& context,
                                     buffer_type     type,
                                     size_t          data_size,
                                     void*           data)
        : context_object(context)
        , m_type(type)
        , m_map_count(0)
        , m_mapped_memory(nullptr)
        , m_size(data_size)
    {}

    hardware_buffer::~hardware_buffer() {}

    void* hardware_buffer::map()
    {
        if (m_map_count == 0) {
            m_mapped_memory = on_map();
        }
        ++m_map_count;
        return m_mapped_memory;
    }

    void hardware_buffer::unmap()
    {
        if (m_map_count == 1) {
            on_unmap();
            m_mapped_memory = nullptr;
        }
        --m_map_count;
    }

    bool hardware_buffer::mapped() const noexcept { return m_map_count != 0; }

    buffer_type hardware_buffer::type() const noexcept { return m_type; }

    size_t hardware_buffer::size() const noexcept { return m_size; }

} // namespace mge