// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <cstdlib>
#include <memory>
namespace mge {

    template <typename T>
    inline void copy_struct(T& target, const T& source)
    {
        memcpy(std::addressof(target),
               std::addressof(source),
               sizeof(T));
    }

    template <typename T>
    inline void copy_struct(T* target, const T& source)
    {
        memcpy(target,
               std::addressof(source),
               sizeof(T));
    }

}
