// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"

namespace mge {

    data_type parse_data_type(std::string_view sv)
    {
        auto v = mge::enum_cast<data_type>(sv);
        if (v.has_value()) {
            return v.value();
        }
        MGE_THROW(illegal_argument) << "Invalid data type definition: " << sv;
    }

} // namespace mge