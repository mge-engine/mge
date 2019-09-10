// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/buffer_type.hpp"
#include <cstdint>
namespace mge {

    /**
     * Base class for hardware buffers. Hardware buffers are used for
     * indices and vertex data.
     *
     * Hardware buffers are <i>context objects</i>, i.e. they can be
     * created only after a context has been created (e.g. a window),
     * and they are tied to the context.
     *
     * Graphics APIs usually assign a type to a buffer, that is whether
     * it is used for vertex data (coordinates), index data (describing
     * which vertices are to be drawn in which order), or constants
     * supplied to the shader pipeline.
     *
     * The <i>buffer usage</i> hints how a buffer is used, and how often
     * its data will be changed, which may affect the way how the graphics
     * API tries to cache data.
     * - The <i>default</i> usage applies no restrictions, but also gives
     *   no further optimization hints to the graphics API.
     * - An <i>immutable</i> usage restricts changes to the buffer, it
     *   may for instance not be mapped for changing it, and the graphics
     *   API may apply optimizations as the buffer content will never
     *   change.
     * - The <i>dynamic</i> usage declares that the buffer content may change
     *   occasionally, the <i>streaming</i> usage declares that buffer content
     *   changes frequently.
     *
     * Finally, each buffer can be created with restrictions on access by
     * the CPU and the GPU. Again, optimizations by the graphics API are
     * possible if the buffer is never read or written by the CPU.
     *
     */
    class MGE_GRAPHICS_EXPORT hardware_buffer : public context_object
    {
    protected:
        /**
         * Construct hardware buffer.
         * @param context       render context
         * @param type          buffer type
         * @param buffer_usage  buffer usage
         */
        hardware_buffer(render_context& context,
                        buffer_type type,
                        usage buffer_usage);

        /**
         * Construct hardware buffer.
         * @param context       render context
         * @param type          buffer type
         * @param buffer_usage  buffer usage
         * @param data          buffer content
         * @param data_size     buffer content size in bytes
         */
        hardware_buffer(render_context& context,
                        buffer_type type,
                        usage buffer_usage,
                        void *data,
                        size_t data_size);

    public:
        /**
         * Destructor.
         */
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
        bool mapped() const noexcept;

        /**
         * Returns whether a buffer is mappable. A buffer cannot be mapped if
         * the CPU access is @c NONE.
         * @return @c true if buffer can be mapped
         */
        bool mappable() const noexcept;

        /**
         * Change policy.
         * @return buffer change policy
         */
        usage buffer_usage() const noexcept { return m_usage; }
    protected:
        virtual void *on_map() = 0;
        virtual void on_unmap() = 0;

        buffer_type  m_type;
        usage m_usage;
        uint32_t m_map_count;
        void *m_mapped_memory;
    };
}
