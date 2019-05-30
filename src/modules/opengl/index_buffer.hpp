#include "mge/graphics/index_buffer.hpp"
#include "opengl.hpp"

namespace opengl {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(mge::render_context& context,
                     mge::data_type type,
                     mge::buffer_usage usage,
                     mge::buffer_access cpu_access,
                     mge::buffer_access gpu_access);
        index_buffer(mge::render_context& context,
                     mge::data_type type,
                     mge::buffer_usage usage,
                     mge::buffer_access cpu_access,
                     mge::buffer_access gpu_access,
                     void *data,
                     size_t data_size);
        virtual ~index_buffer();
        size_t size() const override;
    protected:
        void* on_map() override;
        void on_unmap() override;
    };
}
