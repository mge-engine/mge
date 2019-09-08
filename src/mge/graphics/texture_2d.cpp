#include "mge/graphics/texture_2d.hpp"
namespace mge {
    texture_2d::texture_2d(render_context& context,
                     mge::usage texture_usage,
                     filter_function mag_filter,
                     filter_function min_filter,
                     bool mipmap_use,
                     const image_ref& image)
        :context_object(context)
        ,m_mag_filter(mag_filter)
        ,m_min_filter(min_filter)
        ,m_usage(texture_usage)
        ,m_mipmap_use(mipmap_use)
        ,m_format(image?image->format():image_format::UNKNOWN)
        ,m_extent(image?image->extent():mge::extent(0u,0u))
    {}

}
