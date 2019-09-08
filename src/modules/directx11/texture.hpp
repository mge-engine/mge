#include "mge/graphics/texture.hpp"
#include "dx11.hpp"
#include "win32/com_unique_ptr.hpp"
#include <variant>

namespace dx11 {
    class texture : public mge::texture
    {
    public:
        texture(mge::render_context& context,
                mge::texture_type t,
                mge::usage usage,
                mge::filter_function mag_filter,
                mge::filter_function min_filter,
                bool mipmap_use);
        virtual ~texture();
    private:
        typedef COM_UNIQUE_PTR(ID3D11Texture1D) texture_1d_t;
        typedef COM_UNIQUE_PTR(ID3D11Texture2D) texture_2d_t;
        typedef COM_UNIQUE_PTR(ID3D11Texture3D) texture_3d_t;

        typedef std::variant<texture_1d_t,
                             texture_2d_t,
                             texture_3d_t> texture_t;
        texture_t m_texture;
    };
}
