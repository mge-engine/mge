// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/ui/dllexport.hpp"

namespace mge {

    /**
     * @brief UI context providing UI rendering capabilities
     */
    class MGEUI_EXPORT ui_context
    {
    public:
        ui_context();
        ~ui_context();

        ui_context(const ui_context&) = delete;
        ui_context& operator=(const ui_context&) = delete;
        ui_context(ui_context&&) = delete;
        ui_context& operator=(ui_context&&) = delete;
    };

} // namespace mge
