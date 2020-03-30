// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/hardware_buffer.hpp"
namespace mge {

    /**
     * An index buffer. An index buffer contains integer offsets into
     * a vertex buffer, so primitives may be rendered more efficiently.
     */
    class MGE_GRAPHICS_EXPORT index_buffer : public hardware_buffer
    {
    protected:
        /**
         * Constructor
         * @param context       context of buffer
         * @param type          data type
         * @param usage         buffer usage
         * @param element_count element count
         * @param initial_data  optional initial data
         */
        index_buffer(render_context &context, data_type type, usage usage,
                     size_t element_count, void *initial_data = nullptr);

    public:
        virtual ~index_buffer();

        /**
         * Number of elements.
         * @return number of elements
         */
        size_t element_count() const;

    protected:
        data_type m_type;
    };

} // namespace mge
