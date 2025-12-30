// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/buffer_type.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

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
     */
    class MGEGRAPHICS_EXPORT hardware_buffer : public context_object
    {
    protected:
        /**
         * Construct hardware buffer filled with existing data.
         * @param context       render context
         * @param type          buffer type
         * @param data_size     buffer content size in bytes
         */
        hardware_buffer(render_context& context,
                        buffer_type     type,
                        size_t          data_size);

    public:
        /**
         * Destructor.
         */
        virtual ~hardware_buffer();

        /**
         * Size of buffer in bytes.
         * @return buffer size
         */
        size_t size() const noexcept;

        /**
         * @brief Hardware buffer type.
         *
         * @return buffer type
         */
        buffer_type type() const noexcept;

    protected:
        virtual void* on_map() = 0;
        virtual void  on_unmap() = 0;
        virtual void  on_set_data(void* data, size_t data_size) = 0;

        buffer_type m_type;
        size_t      m_size;
    };
} // namespace mge
