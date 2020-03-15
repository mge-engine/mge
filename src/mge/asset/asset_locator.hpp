// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/component.hpp"
#include "mge/core/path.hpp"
#include "mge/asset/asset.hpp"

namespace mge {
    MGE_DECLARE_REF(asset_locator);

    /**
     * An asset locator locates an asset by name,
     * and retrieves a path that can be used to construct
     * an asset.
     */
    class MGE_ASSET_EXPORT asset_locator
            : public mge::component<asset_locator>
    {
    public:
        asset_locator();
        virtual ~asset_locator();

        /**
         * Locates an asset.
         * @param name  asset name
         * @param type  asset type
         * @return path to construct an asset, empty path if asset
         *              was not found
         */
        virtual path locate(const std::string& name,
                            const asset_type& type) = 0;
    };
}
