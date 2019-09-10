#include "mge/graphics/index_buffer.hpp"
#include "opengl.hpp"

namespace opengl {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(mge::render_context& context,
                     mge::data_type type,
                     mge::usage usage,
                     size_t element_count,
                     void *initial_data);
        virtual ~index_buffer();
        size_t size() const override;
    protected:
        void* on_map() override;
        void on_unmap() override;
    private:
        GLuint m_buffer;
        size_t m_size;
    };
}
