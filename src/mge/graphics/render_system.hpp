// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset.hpp"
#include "mge/core/component.hpp"
#include "mge/core/configurable.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/window_options.hpp"

#include <vector>

namespace mge {

    /**
     * The render system defines the entry point into a specific presentation
     * technology, like OpenGL, DirectX or Vulkan.
     */
    class MGE_GRAPHICS_EXPORT render_system : public component<render_system>,
                                              public configurable
    {
    protected:
        render_system();

    public:
        using monitor_collection_type = std::vector<monitor_ref>;

        virtual ~render_system();

        /**
         * Creates the render system identified by the configured name.
         * The render system is configured using the "render.system" config
         * entries.
         * @return created render system, or invalid reference if unavailable
         */
        static render_system_ref create();

        /**
         * Creates a render system named by given name.
         * Note that the render system is not configured and may not be usable.
         * @param name name of render system, such as "opengl" or "vulkan"
         * @return created render system, or invalid reference if unavailable
         */
        static render_system_ref create(const char *name);

        /**
         * Returns default configured name.
         * @return configured name
         */
        static std::string default_name();

        /**
         * Get all monitors attached to the computer.
         * @return list of monitors
         */
        virtual monitor_collection_type monitors() const = 0;

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
        virtual window_ref create_window(const rectangle &     rect,
                                         const window_options &options) = 0;

        /**
         * Create a window using default settings.
         * @return reference to new window
         */
        window_ref create_window();

        /**
         * Locates a shader of a given type that has a certain name.
         *
         * @param type type
         * @param name shader name, may be a path
         * @return asset shader asset
         */
        virtual asset locate_shader(shader_type type, const char *name) = 0;
    };
} // namespace mge
