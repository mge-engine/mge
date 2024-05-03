// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_data_type.hpp"
#include <iostream>
#include <map>
#include <string_view>

namespace mge {
    uniform_data_type parse_uniform_data_type(std::string_view sv)
    {
        auto udt = mge::enum_cast<uniform_data_type>(sv);
        if (udt.has_value()) {
            return udt.value();
        }
        MGE_THROW(illegal_argument) << "Invalid data type definition: " << sv;
    }

} // namespace mge