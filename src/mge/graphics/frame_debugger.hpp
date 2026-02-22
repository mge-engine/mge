// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {
    class render_context;
    class frame_command_list;

    /**
     * A frame debugger. Frame debuggers such a s renderdoc capture work
     * by hooking into the rendering process and capturing frames for later
     * inspection.
     */
    class MGEGRAPHICS_EXPORT frame_debugger
        : public mge::component<frame_debugger>
    {
    public:
        struct capture_context
        {
            void* context;
            void* window;

            inline bool operator==(const capture_context& other) const noexcept
            {
                return context == other.context && window == other.window;
            }

            inline bool operator!=(const capture_context& other) const noexcept
            {
                return !(*this == other);
            }
        };

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
         * @brief Set the context for capturing.
         *
         * @param ctx capture context
         */
        void set_context(capture_context ctx) noexcept
        {
            on_set_context(ctx);
            m_capture_context = ctx;
        }

        /**
         * @brief Check whether a frame capture is in progress.
         */
        virtual bool capturing() const = 0;

        /**
         * @brief Begin capturing frames.
         */
        virtual void begin_capture() = 0;

        /**
         * @brief End capturing frames.
         */
        virtual void end_capture() = 0;

    protected:
        virtual void on_set_context(capture_context ctx) {}

        capture_context m_capture_context;
    };
} // namespace mge