// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_buffer.hpp"
#include "opengl.hpp"

namespace opengl {
    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(mge::render_context& context,
                      const mge::vertex_layout& layout,
                      mge::usage usage,
                      size_t element_count,
                      void *initial_data = nullptr);
        virtual ~vertex_buffer();
        size_t size() const override;
        GLuint name() const { return m_buffer; }
    protected:
        void* on_map() override;
        void on_unmap() override;
    private:
        GLuint m_buffer;
        size_t m_size;
    };

    inline GLuint gl_vertex_buffer(const mge::vertex_buffer& buf)
    {
        const opengl::vertex_buffer& ogl_buf = static_cast<const opengl::vertex_buffer&>(buf);
        return ogl_buf.name();
    }

    inline GLuint gl_vertex_buffer(const mge::vertex_buffer_ref& buf)
    {
        if (buf) {
            return gl_vertex_buffer(*buf);
        } else {
            return 0;
        }
    }
}
