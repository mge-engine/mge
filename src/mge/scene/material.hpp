// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/scene/dllexport.hpp"
#include "mge/scene/material_type.hpp"

namespace mge {

    /**
     * Abstract base class for all materials.
     */
    class MGESCENE_EXPORT material
    {
    public:
        material() = default;
        material(const material&) = default;
        material(material&&) = default;
        material& operator=(const material&) = default;
        material& operator=(material&&) = default;
        virtual ~material() = default;

        /**
         * Returns the type of this material.
         */
        virtual material_type type() const = 0;
    };

} // namespace mge
