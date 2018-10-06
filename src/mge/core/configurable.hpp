/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
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
         * Configure the object.
         * @param config the configuration
         */
        virtual void configure(const configuration& config) = 0;
    };
}
