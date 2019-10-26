// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>

namespace mge {

    template <typename T>
    struct com_deleter
    {
        typedef T *pointer;
        com_deleter() = default;
        inline void operator()(T *ptr) const { ptr->Release(); }
    };

    template <typename T>
    inline std::unique_ptr<T, mge::com_deleter<T>> make_com_unique_ptr(T* ptr)
    {
        return std::unique_ptr<T, mge::com_deleter<T>>(ptr);
    }

    template <typename T>
    using com_unique_ptr = std::unique_ptr<T, mge::com_deleter<T>>;

}


