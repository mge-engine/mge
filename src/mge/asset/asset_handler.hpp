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
     * @brief Interface for asset handlers.
     *
     * An asset handler is responsible for loading an asset.
     * Assets can be loaded as raw streams, but usually one wants to load
     * an asset in a structured way, e.g. as a image, a mesh, a shader etc.
     *
     */
    class MGEASSET_EXPORT asset_handler : public component<asset_handler>
    {
    public:
        enum class operation_type
        {
            LOAD,
            STORE
        };

        asset_handler() = default;
        virtual ~asset_handler() = default;

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
         * @brief Stores the asset.
         *
         * This writes the asset to the output stream.
         *
         * @param asset asset that is stored
         * @param type asset type
         * @param data asset data to store
         */
        virtual void store(const asset&      asset,
                           const asset_type& type,
                           const std::any&   data) = 0;

        /**
         * @brief Return asset types handled by this handler.
         *
         * @param t operation type (load or store)
         * @return asset types handled by this handler
         */
        virtual std::span<asset_type> handled_types(operation_type t) const = 0;

        /**
         * @brief Return whether this handler can improve the asset type.
         *
         * @param asset asset to be checked, type stored in asset is unreliable
         * @param type prelimary type of the asset, usually a common type
         *
         * @return @c true if this handler can improve the asset type, @c false
         * otherwise
         */
        virtual bool can_improve(const asset&      asset,
                                 const asset_type& type) const;

        /**
         * @brief Improve the asset type.
         *
         * This can be called if @c can_improve returns @c true.
         *
         * @param asset asset to be checked, type stored in asset is unreliable
         * @param type prelimary type of the asset, usually a common type
         *
         * @return improved asset type, or @c asset_type::UNKNOWN if no
         * improvement
         */
        virtual asset_type improve(const asset&      asset,
                                   const asset_type& type) const;

        using component<asset_handler>::create;
        using component<asset_handler>::implementations;
    };

} // namespace mge