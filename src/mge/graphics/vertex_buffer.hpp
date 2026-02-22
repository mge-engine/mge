// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/hardware_buffer.hpp"
#include "mge/graphics/vertex_layout.hpp"

namespace mge {

    /**
     * @brief A vertex buffer.
     * A vertex buffer contains vertices, in a layout defined
     * by the buffer's layout.
     */
    class MGEGRAPHICS_EXPORT vertex_buffer : public hardware_buffer
    {
    protected:
        /**
         * @brief Constructor.
         *
         * @param context render context
         * @param layout vertex layout
         * @param data_size size in bytes
         */
        vertex_buffer(render_context&      context,
                      const vertex_layout& layout,
                      size_t               data_size);

    public:
        ~vertex_buffer();

        /**
         * @brief Get the layout of this buffer.
         *
         * @return layout of this buffer.
         */
        const vertex_layout& layout() const;

        /**
         * @brief Get the element count of this buffer.
         *
         * @return number of elements (vertices)
         */
        size_t element_count() const;

        /**
         * @brief Set the data of the vertex buffer.
         * The actual data transfer will be deferred to prepare frame time.
         *
         * @param data Buffer containing the data to set
         */
        void set_data(const buffer_ref& data);

    private:
        vertex_layout m_layout;
    };
} // namespace mge