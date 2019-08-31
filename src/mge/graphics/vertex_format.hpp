#pragma once 
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/data_type.hpp"
#include <iosfwd>

namespace mge {
 
    class MGE_GRAPHICS_EXPORT vertex_format
    {
    public:
        inline vertex_format()
            :m_type(data_type::UNKNOWN)
            ,m_size(0u)
        {}
        
        inline vertex_format(data_type type,
                            size_t size)
            :m_type(type)
            ,m_size(static_cast<uint8_t>(size))
        {}

        vertex_format(const vertex_format& f) = default;
        vertex_format(vertex_format&& f) = default;

        vertex_format& operator =(const vertex_format& f) = default;
        vertex_format& operator =(vertex_format&& f) = default;

        data_type type() const noexcept { return m_type; }
        uint8_t size() const noexcept { return m_size; }
        size_t binary_size() const;
    private:
        data_type m_type;
        uint8_t   m_size;
    };

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, 
                                                  const vertex_format& fmt);
}