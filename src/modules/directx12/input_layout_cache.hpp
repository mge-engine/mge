// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/core/string_pool.hpp"
#include "mge/graphics/vertex_layout.hpp"

#include <unordered_map>
#include <vector>

namespace mge::dx12 {
    class input_layout_cache
    {
    public:
        input_layout_cache();
        ~input_layout_cache();

        const std::vector<D3D12_INPUT_ELEMENT_DESC>&
        get(const vertex_layout& layout);

    private:
        void initialize_layout(
            const vertex_layout&                   layout,
            std::vector<D3D12_INPUT_ELEMENT_DESC>& desc);
        static DXGI_FORMAT dxgi_format(const vertex_format& format);

        string_pool m_semantic_names;
        std::unordered_map<vertex_layout,
                           std::vector<D3D12_INPUT_ELEMENT_DESC>>
            m_layouts;
    };
} // namespace mge::dx12
