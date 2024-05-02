// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"

#include <iosfwd>
#include <limits>

#ifdef max
#    undef max
#endif

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

        /**
         * @brief Create video mode.
         *
         * @param width         width in pixels
         * @param height        height in pixels
         * @param refresh_rate_ refresh rate in Hz
         */
        video_mode(uint32_t width, uint32_t height, uint32_t refresh_rate_)
            : extent(width, height)
            , refresh_rate(refresh_rate_)
        {}

        video_mode()
            : refresh_rate(ANY_REFRESH_RATE)
        {}
        video_mode(const video_mode&) = default;
        video_mode& operator=(const video_mode&) = default;

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

        bool operator==(const video_mode& m) const
        {
            return this->extent == m.extent && is_refresh_rate(m.refresh_rate);
        }

        bool operator!=(const video_mode& m) const
        {
            return this->extent != m.extent || !is_refresh_rate(m.refresh_rate);
        }

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(), "{}x{}", extent.width, extent.height);
            if (refresh_rate == ANY_REFRESH_RATE) {
                std::format_to(ctx.out(), "@ANY");
            } else {
                std::format_to(ctx.out(), "@{}", refresh_rate);
            }
        }
    };

} // namespace mge