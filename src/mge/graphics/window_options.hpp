// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include <bitset>

namespace mge {
    /**
     * @brief Window options that can be specified when window is created.
     */
    class MGEGRAPHICS_EXPORT window_options
    {
    public:
        enum option
        {
            NONE = 0,
            FULLSCREEN,
            FULLSCREEN_WINDOWED,
            RESIZABLE,
            MAXIMIZED,
            MINIMIZED,
            DECORATED,
            TITLE,
            MAX = TITLE
        };

        window_options() {}

        template <typename... Args> explicit window_options(Args... args)
        {
            window_options::option options[] = {args...};
            for (auto o : options) {
                set_option(o);
            }
        }
        window_options(const window_options& o)
            : m_options(o.m_options)
        {}

        window_options(window_options&& o)
            : m_options(std::move(o.m_options))
        {}

        inline window_options& operator=(const window_options& o)
        {
            m_options = o.m_options;
            return *this;
        }

        inline window_options& operator=(window_options&& o)
        {
            m_options = std::move(o.m_options);
            return *this;
        }

        ~window_options() = default;

        window_options& set_option(option o, bool value = true);
        bool            option_set(option o) const;

        /**
         * Standard window options.
         * @return standard options
         */
        static window_options standard_options();
        /**
         * @brief Fullscreen options.
         *
         * @return options for fullscreen
         */
        static window_options fullscreen_options();

    private:
        std::bitset<(size_t)MAX> m_options;
    };
} // namespace mge
