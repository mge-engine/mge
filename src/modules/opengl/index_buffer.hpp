// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context_base;

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(render_context_base& context,
                     mge::data_type  dt,
                     size_t          data_size);
        virtual ~index_buffer();

        inline GLuint buffer_name() const noexcept
        {
            return m_buffer;
        }

        void on_set_data(void* data, size_t size) override;

    private:
        GLuint m_buffer;
    };

    inline GLuint gl_index_buffer(const mge::index_buffer& buf)
    {
        const opengl::index_buffer& ogl_buf =
            static_cast<const opengl::index_buffer&>(buf);
        return ogl_buf.buffer_name();
    }

    inline GLuint gl_index_buffer(const mge::index_buffer* buf)
    {
        if (buf) {
            return gl_index_buffer(*buf);
        } else {
            return 0;
        }
    }

} // namespace mge::opengl