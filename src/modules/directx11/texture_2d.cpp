#include "texture_2d.hpp"
#include "error.hpp"

namespace dx11 {
    texture_2d::texture_2d(mge::render_context& context,
                           const mge::image_ref& image)
        :mge::texture_2d(context,
                         image)
    {
        if (image) {
            create_texture(image);
        }
    }

    texture_2d::texture_2d(mge::render_context& context)
        :mge::texture_2d(context)
    {}

    texture_2d::~texture_2d()
    {}

    void
    texture_2d::create_texture(const mge::image_ref& image)
    {
        /*
        D3D11_TEXTURE2D_DESC texture_desc;
        texture_desc.Width = extent().width();
        texture_desc.Height = extend().height();
        texture_desc.MipLevels = mipmap_use()?0:1;
        texture_desc.ArraySize = 1;
        */
    }

}
