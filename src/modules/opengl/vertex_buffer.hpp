#include "mge/graphics/vertex_buffer.hpp"
#include "opengl.hpp"

namespace opengl {
    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(mge::render_context& context,
                      const mge::vertex_layout& layout,
                      mge::usage usage,
                      mge::buffer_access cpu_access,
                      mge::buffer_access gpu_access,
                      size_t element_count,
                      void *initial_data = nullptr);
        virtual ~vertex_buffer();
        size_t size() const override;
    protected:
        void* on_map() override;
        void on_unmap() override;
    private:
        GLuint m_buffer;
        size_t m_size;
    };
}