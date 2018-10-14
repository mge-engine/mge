// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/script/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge {

    /**
     * A script engine is the entry point for creating a script
     * and running it.
     */
    class script_engine : public component<script_engine>
    {
    protected:
        /**
         * Constructor.
         */
        script_engine() = default;
    public:
        /**
         * Destructor.
         */
        virtual ~script_engine() = default;
    };
}
