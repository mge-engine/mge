// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "render_context.hpp"
namespace mge::opengl {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context,
                     mge::data_type  dt,
                     size_t          data_size,
                     void*           initial_data);
        virtual ~index_buffer();

    private:
        GLuint m_buffer;
    };

} // namespace mge::opengl