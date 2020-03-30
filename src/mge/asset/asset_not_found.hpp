// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    /**
     * @brief Asset exception.
     *
     * Thrown if an asset is not found.
     */
    class MGE_ASSET_EXPORT asset_not_found : public ::mge::exception
    {
    public:
        asset_not_found();
        asset_not_found(const asset_not_found &e);
        asset_not_found(asset_not_found &&e);
        ~asset_not_found();

        asset_not_found &operator=(const asset_not_found &e);

        template <typename Info> asset_not_found &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> asset_not_found &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };
} // namespace mge
