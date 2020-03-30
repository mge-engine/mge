// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/path.hpp"
namespace mge {

    /**
     * Asset type mapper defines relationship between asset type and loader.
     */
    class MGE_ASSET_EXPORT asset_type_mapper
    {
    public:
        /**
         * Register asset type map.
         * @param type      asset type
         * @param suffixes  comma-separated list of name suffixes
         */
        asset_type_mapper(const asset_type &type, const char *suffixes);
        ~asset_type_mapper();

        static asset_type type(const mge::path &path);
    };
} // namespace mge
