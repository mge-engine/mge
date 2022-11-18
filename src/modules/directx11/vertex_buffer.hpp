#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx11 {
    class vertex_buffer : public mge::vertex_buffer
    {
    public:
        vertex_buffer(render_context&      context,
                      const vertex_layout& layout,
                      size_t               data_size,
                      void*                initial_data);
        ~vertex_buffer();

    protected:
        void* on_map() override;
        void  on_unmap() override;

    private:
        mge::com_unique_ptr<ID3D11Buffer> m_buffer;
    };
} // namespace mge::dx11
