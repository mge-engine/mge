// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/buffer_access.hpp"
#include "mge/graphics/buffer_usage.hpp"
#include "mge/graphics/buffer_type.hpp"
#include <cstdint>
namespace mge {
    /**
     * Base class for hardware buffers. Hardware buffers are used for
     * indices and vertex data.
     */
    class MGE_GRAPHICS_EXPORT hardware_buffer : public context_object
    {
    protected:
        hardware_buffer(render_context& context,
                        buffer_type type,
                        buffer_usage usage,
                        buffer_access cpu_access,
                        buffer_access gpu_access);
    public:
        virtual ~hardware_buffer();
        /**
         * Size of buffer in bytes.
         * @return buffer size
         */
        virtual size_t size() const = 0;

        /**
         * Maps the buffer.
         * @return mapped buffer
         */
        void *map();

        /**
         * Unmaps the buffer.
         */
        void unmap();

        /**
         * Return whether buffer is currently mapped.
         * @return @c true if mapped
         */
        bool mapped() const noexcept { return m_map_count != 0; }
    protected:
        virtual void *on_map() = 0;
        virtual void on_unmap() = 0;

        buffer_type  m_type;
        buffer_usage m_usage;
        buffer_access m_cpu_access;
        buffer_access m_gpu_access;
        uint32_t m_map_count;
        void *m_mapped_memory;
    };
}
