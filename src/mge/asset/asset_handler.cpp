// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_handler.hpp"
#include "mge/asset/asset_type.hpp"

namespace mge {

    MGE_REGISTER_COMPONENT(asset_handler);

    bool asset_handler::can_improve(const asset&      asset,
                                    const asset_type& type) const
    {
        return false;
    }

    asset_type asset_handler::improve(const asset&      asset,
                                      const asset_type& type) const
    {
        return asset_type::UNKNOWN;
    }

} // namespace mge