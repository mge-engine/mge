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

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        GLuint m_buffer;
    };

} // namespace mge::opengl