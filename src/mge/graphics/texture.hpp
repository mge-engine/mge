#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/filter_function.hpp"

namespace mge {

    class MGE_GRAPHICS_EXPORT texture : public context_object
    {
    protected:
        texture(render_context& d, 
                texture_type t,
                filter_function mag_filter,
                filter_function min_filter,
                bool mipmap_use);
    public:
        virtual ~texture() = default;

        texture_type type() const noexcept { return m_type; }
        filter_function mag_filter() const noexcept { return m_mag_filter; }
        filter_function min_filter() const noexcept { return m_min_filter; }
        bool mipmap_use() const noexcept { return m_mipmap_use; }
    protected:
        texture_type m_type;
        filter_function m_mag_filter;
        filter_function m_min_filter;
        bool m_mipmap_use;
    };
}
