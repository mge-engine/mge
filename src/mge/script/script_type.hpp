// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    /**
     * @brief A type as exposed by the integrated scripting language.
     */
    class MGESCRIPT_EXPORT script_type
    {
    protected:
        script_type() = default;

    public:
        virtual ~script_type() = default;
    };
} // namespace mge::script