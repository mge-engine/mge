// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "error.hpp"
#include "mge/graphics/data_type.hpp"

namespace mge::dx12 {

    inline DXGI_FORMAT dx12_format(mge::data_type type)
    {
        switch (type) {
        case mge::data_type::INT16:
        case mge::data_type::UINT16:
            return DXGI_FORMAT_R16_UINT;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            return DXGI_FORMAT_R32_UINT;
        default:
            MGE_THROW(dx12::error) << "Unsupported data type " << type;
        }
    }

} // namespace mge::dx12