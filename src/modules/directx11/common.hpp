// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "error.hpp"
#include "mge/graphics/data_type.hpp"

namespace mge::dx11 {

    inline DXGI_FORMAT dx11_format(mge::data_type type)
    {
        switch (type) {
        case mge::data_type::INT16:
        case mge::data_type::UINT16:
            return DXGI_FORMAT_R16_UINT;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            return DXGI_FORMAT_R32_UINT;
        default:
            MGE_THROW(dx11::error) << "Unsupported data type " << type;
        }
    }

} // namespace mge::dx11