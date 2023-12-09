// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "input_layout_cache.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge::dx11 {

    struct semantic_context
    {
        semantic_context()
            : position_used(false)
        {}

        bool position_used;
    };

    input_layout_cache::input_layout_cache() {}

    input_layout_cache::~input_layout_cache() {}

    D3D11_INPUT_ELEMENT_DESC*
    input_layout_cache::get(const vertex_layout& layout)
    {
        auto i = m_layouts.find(layout);
        if (i == m_layouts.end()) {
            auto insert_result = m_layouts.insert(
                std::make_pair(layout,
                               std::vector<D3D11_INPUT_ELEMENT_DESC>()));
            initialize_layout(layout, insert_result.first->second);
            return insert_result.first->second.data();
        } else {
            return i->second.data();
        }
    }

    static std::string_view
    make_semantic_name(string_pool&              pool,
                       const attribute_semantic& semantic,
                       semantic_context&         context)
    {
        switch (semantic) {
        case attribute_semantic::POSITION:
            return pool.get("POSITION");
        case attribute_semantic::ANY:
            if (!context.position_used) {
                context.position_used = true;
                return pool.get("POSITION");
            }
            [[fallthrough]];
        default:
            MGE_THROW(illegal_state) << "Unsupported semantic: " << semantic;
        }
    }

    DXGI_FORMAT input_layout_cache::dxgi_format(const vertex_format& format)
    {
        switch (format.type()) {
        case data_type::FLOAT:
            switch (format.size()) {
            case 1:
                return DXGI_FORMAT_R32_FLOAT;
            case 2:
                return DXGI_FORMAT_R32G32_FLOAT;
            case 3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported float format size: " << format.size();
            }
        case data_type::INT32:
            switch (format.size()) {
            case 1:
                return DXGI_FORMAT_R32_SINT;
            case 2:
                return DXGI_FORMAT_R32G32_SINT;
            case 3:
                return DXGI_FORMAT_R32G32B32_SINT;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_SINT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported int format size: " << format.size();
            }
        case data_type::UINT32:
            switch (format.size()) {
            case 1:
                return DXGI_FORMAT_R32_UINT;
            case 2:
                return DXGI_FORMAT_R32G32_UINT;
            case 3:
                return DXGI_FORMAT_R32G32B32_UINT;
            case 4:
                return DXGI_FORMAT_R32G32B32A32_UINT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported uint format size: " << format.size();
            }
        default:
            MGE_THROW(illegal_argument)
                << "Unsupported format type: " << format.type();
        }
    } // namespace mge::dx11

    void input_layout_cache::initialize_layout(
        const vertex_layout&                   layout,
        std::vector<D3D11_INPUT_ELEMENT_DESC>& desc)
    {
        size_t           offset = 0;
        semantic_context context;
        for (const auto& element : layout) {
            D3D11_INPUT_ELEMENT_DESC& desc_element = desc.emplace_back();
            desc_element.SemanticName =
                make_semantic_name(m_semantic_names, element.semantic, context)
                    .data();
            desc_element.SemanticIndex = 0;
            desc_element.Format = dxgi_format(element.format);
            desc_element.InputSlot = 0;
            desc_element.AlignedByteOffset = static_cast<UINT>(offset);
            desc_element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            desc_element.InstanceDataStepRate = 0;
            offset += element.format.binary_size();
        }
    }
} // namespace mge::dx11