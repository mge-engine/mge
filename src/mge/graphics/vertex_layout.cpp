#include "mge/graphics/vertex_layout.hpp"

namespace mge {
    vertex_layout::vertex_layout() {}

    vertex_layout::vertex_layout(const std::initializer_list<vertex_format>& l)
    {
        reserve(l.size());
        for (const auto f : l) {
            push_back(f);
        }
    }
} // namespace mge