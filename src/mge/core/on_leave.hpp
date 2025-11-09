// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"

namespace mge {
    /**
     * @brief Execute a callable on leaving the current scope.
     *
     * @tparam Callable
     */
    template <typename Callable> class on_leave
    {
    public:
        on_leave(Callable&& c)
            : m_callable(std::move(c))
        {}

        ~on_leave()
        {
            m_callable();
        }

        Callable m_callable;
    };

} // namespace mge