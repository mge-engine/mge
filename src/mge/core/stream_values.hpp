// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <ostream>

namespace mge {

    inline void stream_values(std::ostream& os)
    {}

    template <typename T, typename... Args>
    inline void stream_values(std::ostream& os,
                              T value,
                              Args... args)
    {
        os << value;
        stream_values(os, args...);
    }
}
