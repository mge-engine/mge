// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/callable.hpp"
#include "mge/core/dllexport.hpp"

#include <functional>

namespace mge {

    /**
     * @brief Class to register a callback to be executed at program exit.
     */
    class MGECORE_EXPORT atexit
    {
    public:
        using callback = std::function<void()>;

        /**
         * @brief Register a callback to be executed at program exit.
         * @param cb callback to be executed
         */
        static void run(callback&& cb);

        /**
         * @brief Stop processing atexit callbacks.
         *
         * Can be called while processing atexit callbacks to stop further
         * processing, or before processing atexit callbacks to prevent
         * processing at all.
         */
        void stop_processing();
    };
} // namespace mge