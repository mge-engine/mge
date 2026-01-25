// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/noncopyable.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_format.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/window_options.hpp"

#include <algorithm>
#include <span>
#include <vector>

namespace mge {
    /**
     * @brief Render system interface.
     *
     * The render system defines a mechanism to produce graphics
     * output, that is:
     *
     * - open and manage windows to present graphics output
     * - manage the capabilities of the render system
     * - create and manage the objects of a graphics pipeline, such as shaders
     *   and buffers
     */
    class MGEGRAPHICS_EXPORT render_system : public component<render_system>
    {
    public:
        /**
         * @brief Capabilities of the render system to check for optional
         * features.
         */
        class MGEGRAPHICS_EXPORT capabilities : public noncopyable
        {
        protected:
            capabilities();

        public:
            virtual ~capabilities();
            /**
             * @brief Get supported shader languages.
             *
             * @return list of supported shader languages
             */
            virtual std::span<const shader_language> shader_languages() const;

            /**
             * @brief Get supported shader formats.
             *
             * @return list of supported shader formats
             */
            virtual std::span<const shader_format> shader_formats() const;
        };

        /**
         * Destructor, will stop frame recording if active.
         */
        virtual ~render_system();

        /**
         * @brief Create window with default settings.
         *
         * @return created window
         */
        window_ref create_window();

        /**
         * @brief Create a window
         * @param extent  window size
         * @param options window options
         * @return created window
         */
        virtual window_ref create_window(const extent&         extent,
                                         const window_options& options) = 0;

        /**
         * @brief Create default render system.
         *
         * @return default render system
         */
        static render_system_ref create();

        /**
         * @brief Create a specific render system.
         *
         * @param implementation
         * @return created render system
         */
        static render_system_ref create(std::string_view implementation);

        /**
         * @brief Get monitors of the system.
         *
         * @return attached monitors
         */
        virtual std::span<monitor_ref> monitors() = 0;

        /**
         * @brief Access render system capabilities.
         *
         * @return render system capabilities
         */
        const mge::render_system::capabilities& system_capabilities() const;

        /**
         * @brief Get the frame debugger available for this render system.
         * @return frame debugger if available
         */
        frame_debugger_ref frame_debugger() const;

    protected:
        std::unique_ptr<mge::render_system::capabilities> m_capabilities;
        frame_debugger_ref                                m_frame_debugger;
    };
} // namespace mge
