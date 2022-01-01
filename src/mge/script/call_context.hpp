// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"

namespace mge::script {

    /**
     * @brief Call context.
     * When calling a method or function, a call context
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        virtual ~call_context();
    };
} // namespace mge::script