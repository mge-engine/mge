// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stream_values.hpp"
#include <sstream>
#include <string>

namespace mge {

    /**
     * @brief Collate string representation of arg
     *
     * @tparam Args argument types
     * @param args arguments
     * @return arguments printed, and collated as string
     */
    template <typename... Args> inline std::string format_string(Args... args)
    {
        std::stringstream ss;
        stream_values(ss, args...);
        return ss.str();
    }
} // namespace mge
