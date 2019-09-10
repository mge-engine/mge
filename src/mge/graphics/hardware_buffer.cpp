// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/hardware_buffer.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stacktrace.hpp"

MGE_USE_IMPORTED_LOG(MGE);

namespace mge {

    hardware_buffer::hardware_buffer(render_context &context,
                                     buffer_type type,
                                     usage usage)
        :context_object(context)
        ,m_type(type)
        ,m_usage(usage)
        ,m_map_count(0)
        ,m_mapped_memory(nullptr)
    {
        if(!mappable()) {
            MGE_THROW(mge::illegal_argument)
                << "Unsupported usage for unmapped buffer";
        }
    }

    hardware_buffer::hardware_buffer(render_context &context,
                                     buffer_type type,
                                     usage usage,
                                     void * /*data*/,
                                     size_t /*data_size*/)
        :context_object(context)
        ,m_type(type)
        ,m_usage(usage)
        ,m_map_count(0)
        ,m_mapped_memory(nullptr)
    {
    }


    hardware_buffer::~hardware_buffer()
    {
        if(mapped()) {
            MGE_WARNING_LOG(MGE) << "Destroying mapped buffer ";
            MGE_WARNING_LOG(MGE) << mge::stacktrace();
        }
    }

    void *
    hardware_buffer::map()
    {
        if(!m_map_count) {
            m_mapped_memory = on_map();
        }
        ++m_map_count;
        return m_mapped_memory;
    }

    void
    hardware_buffer::unmap()
    {
        if(m_map_count == 1) {
            on_unmap();
            m_map_count = 0;
            m_mapped_memory = nullptr;
        } else if(m_map_count > 0) {
            --m_map_count;
        }
    }


    bool
    hardware_buffer::mapped() const noexcept
    {
        return m_map_count != 0;
    }

    bool
    hardware_buffer::mappable() const noexcept
    {
        return m_usage != mge::usage::IMMUTABLE;
    }


}
