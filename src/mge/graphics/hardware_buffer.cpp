// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/hardware_buffer.hpp"

namespace mge {

    hardware_buffer::hardware_buffer(render_context& context,
                                     buffer_type     type,
                                     size_t          data_size)
        : context_object(context)
        , m_type(type)
        , m_size(data_size)
    {}

    hardware_buffer::~hardware_buffer() {}

    buffer_type hardware_buffer::type() const noexcept
    {
        return m_type;
    }

    size_t hardware_buffer::size() const noexcept
    {
        return m_size;
    }

} // namespace mge