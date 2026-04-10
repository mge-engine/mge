// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"

namespace mge {

    /**
     * @brief Install crash dump handler.
     *
     * On Windows, registers an unhandled exception filter that writes
     * a crash dump file. On other platforms, this is a no-op.
     */
    MGECORE_EXPORT void install_dump_handler();

} // namespace mge
