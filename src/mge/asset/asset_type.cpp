// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset_type.hpp"
#include <iostream>

namespace mge {
    const asset_type asset_type::UNKNOWN("unknown", "unknown");

    std::ostream &operator<<(std::ostream &os, const asset_type &t)
    {
        return os << t.type() << "/" << t.subtype();
    }

    asset_type::asset_type() : asset_type(asset_type::UNKNOWN) {}

    namespace string_literals {
        MGE_ASSET_EXPORT asset_type operator""_asset_type(const char *s,
                                                          size_t      sz)
        {
            auto p = std::find(s, s + sz, '/');
            if (p == s + sz) {
                return asset_type::UNKNOWN;
            } else {
                const std::string_view type(s, p - s);
                const std::string_view subtype(p + 1, (s + sz) - (p + 1));
                if (subtype.empty()) {
                    return asset_type::UNKNOWN;
                } else {
                    return asset_type(type, subtype);
                }
            }
        }
    } // namespace string_literals
} // namespace mge
