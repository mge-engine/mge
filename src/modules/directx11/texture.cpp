#include "texture.hpp"
#include "error.hpp"

namespace dx11 {
    texture::texture(mge::render_context& context,
                     mge::texture_type t,
                     mge::usage usage,
                     mge::filter_function mag_filter,
                     mge::filter_function min_filter,
                     bool mipmap_use)
        :mge::texture(context,
                      t,
                      usage,
                      mag_filter,
                      min_filter,
                      mipmap_use)
    {
    }

    texture::~texture()
    {}

}
