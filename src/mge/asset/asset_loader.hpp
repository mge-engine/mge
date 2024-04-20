// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/component.hpp"
#include <any>
#include <functional>
#include <span>

namespace mge {

    /**
     * @brief Interface for asset loaders.
     *
     * An asset loader is responsible for loading an asset.
     * Assets can be loaded as raw streams, but usually one wants to load
     * an asset in a structured way, e.g. as a image, a mesh, a shader etc.
     *
     */
    class MGEASSET_EXPORT asset_loader : public component<asset_loader>
    {
    public:
        asset_loader() = default;
        virtual ~asset_loader() = default;

        /**
         * @brief Loads the asset.
         *
         * This constructs the loaded asset from the input stream and
         * properties.
         *
         * @param asset asset that is loaded
         * @return std::any loaded asset
         */
        virtual std::any load(const asset& asset) = 0;

        /**
         * @brief Return asset types handled by this loader.
         *
         * @return asset types handled by this loader
         */
        virtual std::span<asset_type> handled_types() const = 0;

        using component<asset_loader>::create;
        using component<asset_loader>::implementations;
    };

} // namespace mge