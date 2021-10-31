// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <utility>

namespace mge {

    /**
     * @brief Unprotect constructor of base class.
     *
     * @tparam B base class
     */
    template <typename B> class unprotect_constructor : public B
    {
    public:
        template <typename... Args>
        unprotect_constructor(Args&&... args)
            : B(std::forward<Args>(args)...)
        {}
    };

} // namespace mge