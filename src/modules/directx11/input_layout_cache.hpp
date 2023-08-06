#pragma once
#include "dx11.hpp"
#include "mge/core/string_pool.hpp"
#include "mge/graphics/vertex_layout.hpp"

#include <unordered_map>
#include <vector>

namespace mge::dx11 {
    class input_layout_cache
    {
    public:
        input_layout_cache();
        ~input_layout_cache();

        D3D11_INPUT_ELEMENT_DESC* get(const vertex_layout& layout);

    private:
        string_pool m_semantic_names;
        std::unordered_map<vertex_layout, std::vector<D3D11_INPUT_ELEMENT_DESC>>
            m_layouts;
    };
} // namespace mge::dx11