// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

namespace mge {

    class MGECORE_EXPORT dump
    {
    public:
        /**
         * @brief Install crash dump handler.
         *
         * On Windows, registers an unhandled exception filter that writes
         * a crash dump file. On other platforms, this is a no-op.
         */
        static void install_handler();

        /**
         * @brief Uninstall crash dump handler.
         *
         * On Windows, resets the unhandled exception filter. On other
         * platforms, this is a no-op.
         */
        static void uninstall_handler();
    };

} // namespace mge
