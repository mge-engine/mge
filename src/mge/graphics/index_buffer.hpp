// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/hardware_buffer.hpp"

namespace mge {

    /**
     * An index buffer. An index buffer contains integer offsets into
     * a vertex buffer, so primitives may be rendered more efficiently.
     */
    class MGEGRAPHICS_EXPORT index_buffer : public hardware_buffer
    {
    protected:
        /**
         * @brief Construct a new index buffer object.
         *
         * @param context render context used
         * @param dt      data type
         */
        index_buffer(render_context& context, data_type dt);

        /**
         * @brief Construct a new index buffer object.
         *
         * @param context   render context used
         * @param dt        data type
         * @param data      initial data
         * @param data_size size in bytes of initial data
         */
        index_buffer(render_context& context, data_type dt, void* data, size_t data_size);

    public:
        virtual ~index_buffer();

        /**
         * @brief Get the type of index buffer element.
         *
         * @return type of element
         */
        data_type element_type() const noexcept;

        /**
         * @brief Get the number of elements in the buffer.
         *
         * @return number of elements
         */
        size_t element_count() const;

    private:
        data_type m_data_type;
    };

} // namespace mge