#include "mge/graphics/texture_2d.hpp"
#include "dx11.hpp"
#include "win32/com_unique_ptr.hpp"
#include <variant>

namespace dx11 {
    class texture_2d : public mge::texture_2d
    {
    public:
        texture_2d(mge::render_context& context,
                   mge::usage usage,
                   mge::filter_function mag_filter,
                   mge::filter_function min_filter,
                   bool mipmap_use);
        virtual ~texture_2d();
    private:
        typedef COM_UNIQUE_PTR(ID3D11Texture2D) texture_t;
        texture_t m_texture;
    };
}
