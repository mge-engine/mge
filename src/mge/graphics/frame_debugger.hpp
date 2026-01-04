// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/component.hpp"

namespace mge {
    class render_context;
    class frame_command_list;

    /**
     * A frame debugger. Frame debuggers such a s renderdoc capture work
     * by hooking into the rendering process and capturing frames for later
     * inspection.
     */
    class MGEGRAPHICS_EXPORT frame_debugger : public mge::component<frame_debugger>
    {
    public:
        /**
         * @brief Constructs a frame debugger for the given render context.
         *
         * @param context render context
         */
        explicit frame_debugger();

        /**
         * @brief Destroys the frame debugger.
         */
        virtual ~frame_debugger() override;
    };
} // namespace mge