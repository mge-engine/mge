// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/asset/asset_access.hpp"
#include "mge/core/component.hpp"
#include "mge/core/configurable.hpp"
#include "mge/core/path.hpp"
namespace mge {
    MGE_DECLARE_REF(asset_access_factory);

    class MGE_ASSET_EXPORT asset_access_factory 
        : public component<asset_access_factory>
        , public configurable
    {
    public:
        asset_access_factory() = default;
        virtual ~asset_access_factory() = default;
        
        virtual asset_access_ref create_asset_access(const mge::path& p) = 0;
        
        void set_mountpoint(const mge::path mountpoint);
        const mge::path& mountpoint() const noexcept { return m_mountpoint; }
    private:
        mge::path m_mountpoint;
    };
}