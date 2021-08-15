// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/graphics_fwd.hpp"
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
    };
} // namespace mge
