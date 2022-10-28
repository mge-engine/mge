// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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
        class capabilities : public noncopyable
        {
        protected:
            capabilities();

        public:
            ~capabilities();
            /// @brief  list of supported shader languages
            using shader_language_list = std::vector<shader_language>;
            /// @brief  list of supported shader formats
            using shader_format_list = std::vector<shader_format>;

            /**
             * @brief Get supported shader languages.
             *
             * @return list of supported shader languages
             */
            const shader_language_list& shader_languages() const;

            /**
             * @brief Get supported shader formats.
             *
             * @return list of supported shader formats
             */
            const shader_format_list& shader_formats() const;

        protected:
            shader_language_list m_shader_languages;
            shader_format_list   m_shader_formats;
        };

        virtual ~render_system() = default;

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

        using monitor_collection = std::vector<monitor_ref>;

        /**
         * @brief Get monitors of the system.
         *
         * @return attached monitors
         */
        virtual monitor_collection monitors() = 0;

        /**
         * @brief Access render system capabilities.
         *
         * @return render system capabilities
         */
        const mge::render_system::capabilities& system_capabilities() const;

    protected:
        std::unique_ptr<mge::render_system::capabilities> m_capabilities;
    };
} // namespace mge
