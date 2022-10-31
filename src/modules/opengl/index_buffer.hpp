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

        inline GLuint buffer_name() const noexcept { return m_buffer; }

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        GLuint m_buffer;
    };

    inline GLuint gl_index_buffer(const mge::index_buffer& buf)
    {
        const opengl::index_buffer& ogl_buf =
            static_cast<const opengl::index_buffer&>(buf);
        return ogl_buf.buffer_name();
    }

    inline GLuint gl_index_buffer(const mge::index_buffer_ref& buf)
    {
        if (buf) {
            return gl_index_buffer(*buf);
        } else {
            return 0;
        }
    }

} // namespace mge::opengl