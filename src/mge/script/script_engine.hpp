// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/component.hpp"
#include "mge/script/dllexport.hpp"
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
         * Create a new script context, inheriting global definitions.
         * @return new context
         */
        virtual script_context_ref create_context() = 0;

        /**
         * Destructor.
         */
        virtual ~script_engine() = default;
    };
} // namespace mge
