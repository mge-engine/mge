#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_format.hpp"
#include <vector>

namespace mge {

    class MGE_GRAPHICS_EXPORT vertex_layout 
        : public std::vector<vertex_format>
    {
    public:
        vertex_layout();
        vertex_layout(const vertex_layout& l) = default;
        vertex_layout(vertex_layout&& l) = default;
        ~vertex_layout() = default;
        vertex_layout& operator =(const vertex_layout& l) = default;
        vertex_layout& operator =(vertex_layout&& l) = default;

        size_t binary_size() const;
    };

}