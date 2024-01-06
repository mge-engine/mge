// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/topology.hpp"
namespace mge::dx11 {
    inline constexpr D3D11_PRIMITIVE_TOPOLOGY to_dx11_topology(mge::topology t)
    {
        switch (t) {
        case mge::topology::TRIANGLES:
            return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case mge::topology::TRIANGLE_STRIP:
            return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case mge::topology::LINES:
            return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        case mge::topology::LINE_STRIP:
            return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
        default:
            return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        }
    }
} // namespace mge::dx11