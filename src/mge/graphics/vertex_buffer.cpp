// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {
    vertex_buffer::vertex_buffer(render_context&      context,
                                 const vertex_layout& layout,
                                 size_t               data_size)
        : hardware_buffer(context, buffer_type::VERTEX, data_size)
        , m_layout(layout)
    {}

    vertex_buffer::~vertex_buffer() {}

    const vertex_layout& vertex_buffer::layout() const
    {
        return m_layout;
    }

    size_t vertex_buffer::element_count() const
    {
        return size() / m_layout.binary_size();
    }

    void vertex_buffer::set_data(const buffer_ref& data)
    {
        if (!data || data->empty()) {
            return;
        }
        if (data->size() != size()) {
            MGE_THROW(illegal_state)
                << "Data size " << data->size()
                << " does not match buffer size " << size();
        }
        auto self = this;
        context().prepare_frame(
            [self, data]() { self->on_set_data(data->data(), data->size()); });
    }

} // namespace mge