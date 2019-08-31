#include "mge/graphics/vertex_layout.hpp"

namespace mge {
    vertex_layout::vertex_layout()
    {}

    size_t
    vertex_layout::binary_size() const
    {
        size_t result = 0;
        for(const auto& f : *this) {
            result += f.binary_size();
        }
        return result;
    }
}