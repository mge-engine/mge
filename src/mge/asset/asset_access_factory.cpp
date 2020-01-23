// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_access_factory.hpp"
namespace mge {

    MGE_REGISTER_COMPONENT(asset_access_factory);

    void
    asset_access_factory::set_mountpoint(const mge::path& path)
    {
        m_mountpoint = path;
    }

}