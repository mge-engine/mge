// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/window_options.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/core/configurable.hpp"
#include "mge/core/component.hpp"
#include <vector>
namespace mge {

    /**
     * The render system defines the entry point into a specific presentation
     * technology, like OpenGL, DirectX or Vulkan.
     */
    class MGE_GRAPHICS_EXPORT render_system
            : public component<render_system>
            , public configurable
    {
    protected:
        render_system();
    public:
        typedef std::vector<monitor_ref> monitor_collection_t;

        virtual ~render_system();

        /**
         * Creates the render system identified by the configured name.
         * @return created render system, or invalid reference if unavailable
         */
        static render_system_ref create();

        /**
         * Returns default configured name.
         * @return configured name
         */
        static std::string default_name();

        /**
         * Get all monitors attached to the computer.
         * @return list of monitors
         */
        virtual monitor_collection_t monitors() const = 0;

        /**
         * Retrieve the primary monitor.
         * @return primary monitor
         */
        virtual monitor_ref primary_monitor() const = 0;

        /**
         * Create a window.
         * @param rect window rect defining size
         * @param options window options (border, title etc.)
         * @return reference to new window
         */
        virtual window_ref create_window(const rectangle& rect,
                                         const window_options& options) = 0;

        /**
         * Create a window using default settings.
         * @return reference to new window
         */
        window_ref create_window();
    };
}
