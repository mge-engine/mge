#include "mge/graphics/texture_2d.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    texture_2d::texture_2d(render_context& context,
                           const image_ref& image)
        :context_object(context)
        ,m_mag_filter(mge::filter_function::NEAREST)
        ,m_min_filter(mge::filter_function::NEAREST)
        ,m_usage(mge::usage::DEFAULT)
        ,m_mipmap_use(false)
        ,m_format(image?image->format():image_format::UNKNOWN)
        ,m_extent(image?image->extent():mge::extent(0u,0u))
    {
        if(!image) {
            MGE_THROW(mge::illegal_argument) << "Argument 'image' must not be null";
        }
    }

    texture_2d::texture_2d(render_context& context)
        :context_object(context)
        ,m_mag_filter(mge::filter_function::NEAREST)
        ,m_min_filter(mge::filter_function::NEAREST)
        ,m_usage(mge::usage::DYNAMIC)
        ,m_mipmap_use(false)
        ,m_format(image_format::UNKNOWN)
        ,m_extent(mge::extent(0u,0u))
    {}
}
