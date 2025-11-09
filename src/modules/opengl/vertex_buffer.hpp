// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/vertex_buffer.hpp"
#include "opengl.hpp"

namespace mge::opengl {
    class render_context;

    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(render_context&           context,
                      const mge::vertex_layout& layout,
                      size_t                    data_size,
                      void*                     data);
        virtual ~vertex_buffer();

        GLuint buffer_name() const noexcept
        {
            return m_buffer;
        }

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        GLuint m_buffer;
    };

    inline GLuint gl_vertex_buffer(const mge::vertex_buffer& buf)
    {
        const opengl::vertex_buffer& ogl_buf =
            static_cast<const opengl::vertex_buffer&>(buf);
        return ogl_buf.buffer_name();
    }

    inline GLuint gl_vertex_buffer(const mge::vertex_buffer_ref& buf)
    {
        if (buf) {
            return gl_vertex_buffer(*buf);
        } else {
            return 0;
        }
    }

} // namespace mge::opengl