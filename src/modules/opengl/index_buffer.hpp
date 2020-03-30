// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/index_buffer.hpp"
#include "opengl.hpp"

namespace opengl {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(mge::render_context &context, mge::data_type type,
                     mge::usage usage, size_t element_count,
                     void *initial_data);
        virtual ~index_buffer();
        size_t size() const override;
        GLuint name() const { return m_buffer; }

    protected:
        void *on_map() override;
        void  on_unmap() override;

    private:
        GLuint m_buffer;
        size_t m_size;
    };

    inline GLuint gl_index_buffer(const mge::index_buffer &buf)
    {
        const opengl::index_buffer &ogl_buf =
            static_cast<const opengl::index_buffer &>(buf);
        return ogl_buf.name();
    }

    inline GLuint gl_index_buffer(const mge::index_buffer_ref &buf)
    {
        if (buf) {
            return gl_index_buffer(*buf);
        } else {
            return 0;
        }
    }

} // namespace opengl
