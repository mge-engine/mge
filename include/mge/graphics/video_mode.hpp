// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"

#include <iosfwd>
#include <numeric>

namespace mge {

    /**
     * @brief A video mode.
     */
    class video_mode
    {
    public:
        /**
         * @brief Pseudo refresh rate that allows any refresh rate.
         */
        static constexpr uint32_t ANY_REFRESH_RATE =
            std::numeric_limits<uint32_t>::max();

        video_mode() : refresh_rate(ANY_REFRESH_RATE) {}
        video_mode(const video_mode &) = default;
        video_mode &operator=(const video_mode &) = default;

        mge::extent extent;       //!< width and height
        uint32_t    refresh_rate; //!< refresh rate in Hz

        bool is_refresh_rate(uint32_t rate) const
        {
            if (refresh_rate == ANY_REFRESH_RATE || rate == ANY_REFRESH_RATE) {
                return true;
            } else {
                return refresh_rate == rate;
            }
        }

        bool operator==(const video_mode &m) const
        {
            return this->extent == m.extent && is_refresh_rate(m.refresh_rate);
        }

        bool operator!=(const video_mode &m) const
        {
            return this->extent != m.extent || !is_refresh_rate(m.refresh_rate);
        }
    };

    MGEGRAPHICS_EXPORT std::ostream &operator<<(std::ostream &    os,
                                                const video_mode &m);

} // namespace mge