// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_source.hpp"
namespace mge {

    MGE_REGISTER_COMPONENT(asset_source);

    void asset_source::set_mount_point(const mge::path& path)
    {
        on_set_mount_point(path);
        m_mount_point = path;
    }

    void asset_source::set_mode(access_mode mode)
    {
        if (mode != m_mode) {
            mode = on_set_access_mode(mode);
        }
        m_mode = mode;
    }

    void asset_source::on_set_mount_point(const mge::path& path) {}

    asset_source::access_mode asset_source::on_set_access_mode(access_mode mode)
    {
        return mode;
    }
} // namespace mge