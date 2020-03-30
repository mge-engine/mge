// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/component.hpp"
#include <any>
#include <functional>

namespace mge {

    /**
     * An asset loader loads an asset and instantiates an
     * object based on the asset's content.
     */
    class MGE_ASSET_EXPORT asset_loader : public component<asset_loader>
    {
    public:
        asset_loader()          = default;
        virtual ~asset_loader() = default;
        /**
         * Load the asset.
         * @param a asset reference
         * @return pointer to loaded asset
         */
        virtual std::any load(const asset &a) const = 0;

        /**
         * Retrieve the handled asset types.
         * @param f callback invoked for each type
         */
        virtual void
        handled_types(std::function<void(const asset_type &)> &&f) const = 0;

        /**
         * Retrieve loader for asset.
         * @param a asset
         * @return asset loader
         */
        static asset_loader_ref loader(const asset &a);
    };
} // namespace mge
