// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/index_buffer.hpp"

namespace mge {
    index_buffer::index_buffer(render_context &context,
                               buffer_usage usage,
                               buffer_access cpu_access,
                               buffer_access gpu_access)
        :hardware_buffer(context,
                         buffer_type::INDEX_BUFFER,
                         usage,
                         cpu_access,
                         gpu_access)
    {}

    index_buffer::index_buffer(render_context &context,
                               buffer_usage usage,
                               buffer_access cpu_access,
                               buffer_access gpu_access,
                               void *data,
                               size_t data_size)
        :hardware_buffer(context,
                         buffer_type::INDEX_BUFFER,
                         usage,
                         cpu_access,
                         gpu_access)
    {}

    index_buffer::~index_buffer()
    {}

}
