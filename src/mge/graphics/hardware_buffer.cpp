// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/hardware_buffer.hpp"

namespace mge {

    hardware_buffer::hardware_buffer(render_context& context, buffer_type type)
        : context_object(context)
        , m_type(type)
        , m_map_count(0)
        , m_mapped_memory(nullptr)
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

} // namespace mge