// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <functional>
namespace mge {

    template< class R, class... Args >
    inline void clear_function(std::function<R(Args...)>& f)
    {
        f = nullptr;
    }
}
