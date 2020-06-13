// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <ostream>

namespace mge {

    inline void stream_values(std::ostream &os) {}

    /**
     * @brief Stream a list of values into a stream.
     *
     * @tparam T    first argument type
     * @tparam Args argument types
     * @param os    output stream
     * @param value first value
     * @param args  values
     */
    template <typename T, typename... Args>
    inline void stream_values(std::ostream &os, T value, Args... args)
    {
        os << value;
        stream_values(os, args...);
    }
} // namespace mge
