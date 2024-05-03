// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/attribute_semantic.hpp"
#include "mge/core/stdexceptions.hpp"
#include <iostream>

namespace mge {

    attribute_semantic parse_attribute_semantic(std::string_view sv)
    {
        auto v = mge::enum_cast<attribute_semantic>(sv);
        if (v.has_value()) {
            return v.value();
        }
        MGE_THROW(illegal_state) << "Invalid attribute semantic " << sv;
    }
} // namespace mge