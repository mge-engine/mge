// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

namespace mge {

    class configuration;

    /**
     * An object that can be configured.
     */
    class MGE_CORE_EXPORT configurable
    {
    public:
        virtual ~configurable();

        /**
         * Configure the object. The default implementation
         * does nothing.
         * @param config the configuration
         */
        virtual void configure(const configuration &config);
    };
} // namespace mge
