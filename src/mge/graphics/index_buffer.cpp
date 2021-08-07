// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/index_buffer.hpp"

namespace mge {

    index_buffer::index_buffer(render_context& context, data_type dt, size_t data_size, void* data)
        : hardware_buffer(context, buffer_type::INDEX, data_size, data)
        , m_data_type(dt)
    {}

    index_buffer::~index_buffer() {}

    data_type index_buffer::element_type() const noexcept { return m_data_type; }

    size_t index_buffer::element_count() const { return size() / data_type_size(m_data_type); }

} // namespace mge