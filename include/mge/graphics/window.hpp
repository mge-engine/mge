// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/extent.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/window_options.hpp"
#include <memory>
namespace mge {

    /**
     * @brief A window.
     */
    class MGEGRAPHICS_EXPORT window
    {
    protected:
        /**
         * @brief Create a window.
         * @param extent window size
         * @param options   window options
         */
        window(const extent &extent, const window_options &options);


    public:
        virtual ~window();
    };
} // namespace mge
