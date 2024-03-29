// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/video_mode.hpp"

#include <string_view>
#include <vector>
namespace mge {

    class MGEGRAPHICS_EXPORT monitor
    {
    public:
        /**
         * @brief Collection of video modes.
         */
        using video_mode_collection = std::vector<video_mode>;

        virtual ~monitor();

        /**
         * @brief Monitor name.
         *
         * @return monitor name
         */
        virtual std::string_view name() const = 0;

        /**
         * @brief Return whether this monitor is primary.
         *
         * @return @c true if this monitor is the primary monitor
         */
        virtual bool primary() const = 0;

        /**
         * @brief Physical size in mm.
         *
         * @return physical size
         */
        virtual extent physical_size() const = 0;

        /**
         * @brief Get current video mode.
         *
         * @return video mode currently set
         */
        virtual video_mode current_mode() const = 0;

        /**
         * @brief Return supported video modes.
         *
         * @return collection of video modes
         */
        virtual video_mode_collection supported_modes() const = 0;
    };
} // namespace mge