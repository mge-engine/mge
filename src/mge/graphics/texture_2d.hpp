#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/filter_function.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/extent.hpp"

namespace mge {

    class MGE_GRAPHICS_EXPORT texture_2d : public context_object
    {
    protected:
        texture_2d(render_context& d,
                   const image_ref& image);
        texture_2d(render_context& d);
    public:
        virtual ~texture_2d() = default;

        constexpr texture_type type() const noexcept { return mge::texture_type::TEXTURE_2D; }
        filter_function mag_filter() const noexcept { return m_mag_filter; }
        filter_function min_filter() const noexcept { return m_min_filter; }
        bool mipmap_use() const noexcept { return m_mipmap_use; }
        usage usage() const noexcept { return m_usage; }
        image_format format() const noexcept { return m_format; }
        const mge::extent& extent() const noexcept { return m_extent; }
    protected:
        filter_function m_mag_filter;
        filter_function m_min_filter;
        mge::usage m_usage;
        bool m_mipmap_use;
        image_format m_format;
        mge::extent m_extent;
    };
}
