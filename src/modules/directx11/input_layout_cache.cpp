#include "input_layout_cache.hpp"

namespace mge::dx11 {
    input_layout_cache::input_layout_cache() {}

    input_layout_cache::~input_layout_cache() {}

    D3D11_INPUT_ELEMENT_DESC*
    input_layout_cache::get(const vertex_layout& layout)
    {
        auto i = m_layouts.find(layout);
        if (i == m_layouts.end()) {
            return nullptr;
        } else {
            return i->second.data();
        }
    }
} // namespace mge::dx11