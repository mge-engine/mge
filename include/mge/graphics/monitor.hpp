// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"

#include <string_view>
namespace mge {

    class MGEGRAPHICS_EXPORT monitor
    {
    public:
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
        bool primary() const;

        /**
         * @brief Physical size in mm.
         *
         * @return physical size
         */
        virtual extent physical_size() const = 0;
    };
} // namespace mge