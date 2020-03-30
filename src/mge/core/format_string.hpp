// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stream_values.hpp"
#include <sstream>
#include <string>

namespace mge {

    template <typename... Args> inline std::string format_string(Args... args)
    {
        std::stringstream ss;
        stream_values(ss, args...);
        return ss.str();
    }
} // namespace mge
