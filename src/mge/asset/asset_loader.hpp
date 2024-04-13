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

        virtual std::any load(const asset& asset) = 0;

        virtual std::span<asset_type> handled_types() const = 0;
    };

} // namespace mge