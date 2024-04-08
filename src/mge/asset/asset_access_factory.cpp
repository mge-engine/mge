// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_access_factory.hpp"
namespace mge {

    MGE_REGISTER_COMPONENT(asset_access_factory);

    void asset_access_factory::set_mount_point(const mge::path& path)
    {
        on_set_mount_point(path);
        m_mount_point = path;
    }

    void asset_access_factory::on_set_mount_point(const mge::path& path) {}
} // namespace mge