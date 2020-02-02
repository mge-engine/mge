// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <functional>
#include "mge/core/dllexport.hpp"
#include "mge/core/void_function.hpp"

namespace mge {
    /**
     * An @c async_executor executes a function asynchronously, usually in
     * a different thread than the current one.
     */
    class MGE_CORE_EXPORT async_executor
    {
    public:
        /**
         * Destructor.
         */
        virtual ~async_executor() = default;

        /**
         * Executes a function asynchronously.
         * @param f function to execute
         */
        virtual void await(const void_function& f) = 0;

        /**
         * Get a synchronous executor.
         * @return executor that executes the given function synchronously
         */
        static async_executor& synchronous_executor();
    };
}
