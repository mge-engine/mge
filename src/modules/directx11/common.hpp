// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "error.hpp"
#include "mge/graphics/usage.hpp"

namespace dx11 {
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

    inline D3D11_USAGE dx11_usage(mge::usage policy)
    {
        switch (policy) {
        case mge::usage::DEFAULT:
            return D3D11_USAGE_DEFAULT;
        case mge::usage::DYNAMIC:
            return D3D11_USAGE_DYNAMIC;
        case mge::usage::STAGING:
            return D3D11_USAGE_STAGING;
        case mge::usage::IMMUTABLE:
            return D3D11_USAGE_IMMUTABLE;
        }
        return D3D11_USAGE_DEFAULT;
    }

} // namespace dx11
