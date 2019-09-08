#include "texture_2d.hpp"
#include "error.hpp"

namespace dx11 {
    texture_2d::texture_2d(mge::render_context& context,
                           mge::usage usage,
                           mge::filter_function mag_filter,
                           mge::filter_function min_filter,
                           bool mipmap_use)
        :mge::texture_2d(context,
                         usage,
                         mag_filter,
                         min_filter,
                         mipmap_use)
    {}

    texture_2d::~texture_2d()
    {}

}
