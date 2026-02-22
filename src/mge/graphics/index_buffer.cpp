// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/index_buffer.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    index_buffer::index_buffer(render_context& context,
                               data_type       dt,
                               size_t          data_size)
        : hardware_buffer(context, buffer_type::INDEX, data_size)
        , m_data_type(dt)
    {}

    index_buffer::~index_buffer() {}

    data_type index_buffer::element_type() const noexcept
    {
        return m_data_type;
    }

    size_t index_buffer::element_count() const
    {
        return size() / data_type_size(m_data_type);
    }

    void index_buffer::set_data(const buffer_ref& data)
    {
        if (!data || data->empty()) {
            return;
        }
        if (data->size() > size()) {
            MGE_THROW(illegal_state) << "Data size " << data->size()
                                     << " exceeds buffer size " << size();
        }
        auto self = this;
        context().prepare_frame(
            [self, data]() { self->on_set_data(data->data(), data->size()); });
    }

} // namespace mge