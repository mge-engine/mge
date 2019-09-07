#include "mge/graphics/texture.hpp"
namespace mge {
    texture::texture(render_context& context, 
                     texture_type t,
                     usage texture_usage,
                     filter_function mag_filter,
                     filter_function min_filter,
                     bool mipmap_use)
        :context_object(context)
        ,m_type(t)
        ,m_mag_filter(mag_filter)
        ,m_min_filter(min_filter)
        ,m_usage(texture_usage)
        ,m_mipmap_use(mipmap_use)
    {}

}
