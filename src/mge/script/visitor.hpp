// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;
    };
} // namespace mge::script