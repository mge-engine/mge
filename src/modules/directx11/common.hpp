#pragma once
#include "dx11.hpp"
#include "mge/graphics/buffer_access.hpp"
#include "mge/graphics/usage.hpp"
#include "mge/graphics/buffer_access.hpp"

namespace dx11 {
    inline  DXGI_FORMAT dx11_format(mge::data_type type)
    {
        switch(type) {
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
        switch(policy) {
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

    inline UINT dx11_access(mge::buffer_access access)
    {
        switch(access) {
        case mge::buffer_access::NONE:
            return 0;
        case mge::buffer_access::READ:
            return D3D11_CPU_ACCESS_READ;
        case mge::buffer_access::WRITE:
            return D3D11_CPU_ACCESS_WRITE;
        case mge::buffer_access::READ_WRITE:
            return D3D11_CPU_ACCESS_WRITE;
        }
        return 0;
    }
}
