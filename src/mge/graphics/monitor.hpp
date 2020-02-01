// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/video_mode.hpp"
#include "mge/graphics/gamma_ramp.hpp"

#include <vector>

namespace mge {

    /**
     * Information on a connected monitor.
     */
    class MGE_GRAPHICS_EXPORT monitor
    {
    protected:
        /**
         * Constructor.
         */
        monitor();
    public:
        /// Type of video mode collection.
        using video_mode_collection_type = std::vector<video_mode>;

        /**
         * Destructor.
         */
        virtual ~monitor() = default;

        /**
         * Get monitor name.
         * @return name
         */
        const std::string& name() const;

        /**
         * Return whether this monitor is primary.
         * @return @c true if this monitor is primary
         */
        bool primary() const;
        /**
         * Width in mm.
         * @return width
         */
        virtual unsigned int physical_width() const = 0;
        /**
         * Height in mm.
         * @return height
         */
        virtual unsigned int physical_height() const = 0;
        /**
         * Monitor gamma ramp.
         * @return gamma ramp
         */
        virtual gamma_ramp gamma_ramp() const = 0;

        /**
         * Supported video modes.
         * @return vector filled with supported video modes
         */
        virtual video_mode_collection_type video_modes() const = 0;

        /**
         * Sets a video mode.
         * @param mode video mode
         */
        virtual void set_video_mode(const video_mode& mode) = 0;

        /**
         * Get current video mode.
         * @return current video mode
         */
        virtual video_mode current_video_mode() const = 0;
    protected:
        monitor(const std::string& name,
                bool primary);
        std::string m_name;
        bool m_primary;
    };
}
