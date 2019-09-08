#include "mge/graphics/texture_2d.hpp"
namespace mge {
    texture_2d::texture_2d(render_context& context,
                     mge::usage texture_usage,
                     filter_function mag_filter,
                     filter_function min_filter,
                     bool mipmap_use)
        :context_object(context)
        ,m_mag_filter(mag_filter)
        ,m_min_filter(min_filter)
        ,m_usage(texture_usage)
        ,m_mipmap_use(mipmap_use)
        ,m_format(image_format::UNKNOWN)
    {}

}
