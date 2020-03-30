// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <functional>
namespace mge {

    /**
     * @brief Reset a @c std::function instance.
     *
     * @tparam R        function result
     * @tparam Args     function arguments
     * @param f         function to reset
     */
    template <class R, class... Args>
    inline void clear_function(std::function<R(Args...)> &f)
    {
        f = nullptr;
    }
} // namespace mge
