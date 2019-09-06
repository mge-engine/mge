#include "dx11.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx11 {

    class index_buffer : public mge::index_buffer
    {
    public:
        index_buffer(mge::render_context& context,
                     mge::data_type type,
                     mge::buffer_change_policy change_policy,
                     mge::buffer_access cpu_access,
                     mge::buffer_access gpu_access,
                     size_t element_count,
                     void  *initial_data);
        virtual ~index_buffer();
        size_t size() const noexcept override;
    protected:
        void *on_map() override;
        void on_unmap() override;
    private:
        void create_buffer(void *data);

        COM_UNIQUE_PTR(ID3D11Buffer) m_buffer;
        DXGI_FORMAT m_format;
        size_t m_size;
    };

}
