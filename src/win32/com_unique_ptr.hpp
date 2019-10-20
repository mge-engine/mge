// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>
#include "wrl/client.h"

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
}

#define COM_UNIQUE_PTR(T) std::unique_ptr<T, mge::com_deleter<T>>
#define COM_PTR(T) Microsoft::WRL::ComPtr<T>


