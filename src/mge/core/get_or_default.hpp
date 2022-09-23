// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
namespace mge {

    constexpr inline auto&
    get_or_default(const auto& map, const auto& key, const auto& default_val)
    {
        const auto itr = map.find(key);
        return itr == map.cend() ? default_val : itr->second;
    }

} // namespace mge