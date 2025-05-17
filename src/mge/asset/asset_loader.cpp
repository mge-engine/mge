// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_type.hpp"

namespace mge {

    MGE_REGISTER_COMPONENT(asset_loader);

    bool asset_loader::can_improve(const asset&      asset,
                                   const asset_type& type) const
    {
        return false;
    }

    asset_type asset_loader::improve(const asset&      asset,
                                     const asset_type& type) const
    {
        return asset_type::UNKNOWN;
    }

} // namespace mge