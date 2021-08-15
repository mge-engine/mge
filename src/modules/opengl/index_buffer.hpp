// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context;

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context& context,
                     mge::data_type  dt,
                     size_t          data_size,
                     void*           initial_data);
        virtual ~index_buffer();

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        GLuint m_buffer;
    };

} // namespace mge::opengl