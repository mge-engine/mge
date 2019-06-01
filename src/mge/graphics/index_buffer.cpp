// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/index_buffer.hpp"

namespace mge {
    index_buffer::index_buffer(render_context &context,
                               data_type type,
                               buffer_usage usage,
                               buffer_access cpu_access,
                               buffer_access gpu_access,
                               size_t element_count,
                               void *initial_data)
        :hardware_buffer(context,
                         buffer_type::INDEX_BUFFER,
                         usage,
                         cpu_access,
                         gpu_access,
                         initial_data,
                         data_type_size(type) * element_count)
        ,m_type(type)
    {}

    index_buffer::~index_buffer()
    {}

}
