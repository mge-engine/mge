// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/core/semantic_version.hpp"

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

        /**
         * @brief Get the name of the frame debugger.
         *
         * @return name of the frame debugger
         */
        virtual std::string_view name() const noexcept = 0;

        /**
         * @brief Get the version of the frame debugger.
         *
         * @return semantic version
         */
        virtual semantic_version version() const noexcept = 0;

        /**
         * @brief Configure the frame debugger.
         */
        virtual void configure() = 0;

        /**
         * @brief Start capturing frames.
         */
        virtual void start_capture() = 0;

        /**
         * @brief End capturing frames.
         */
        virtual void end_capture() = 0;

        /**
         * @brief Capture a frame.
         */
        virtual void capture() = 0;

        /**
         * @brief Get whether a frame is currently being captured.
         * @return true if a frame is being captured
         */
        virtual bool capturing() const = 0;
    };
} // namespace mge