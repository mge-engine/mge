// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/reflection/dllexport.hpp"

#include <span>

namespace mge::reflection {

    /**
     * A reflector exposes types and their details for reflection.
     */
    class MGEREFLECTION_EXPORT reflector : public mge::component<reflector>
    {
    public:
        reflector();
        ~reflector();

        /**
         * Expose the types to reflection.
         */
        virtual void reflect() const = 0;

        /**
         * @brief Get the dependencies of this reflector.
         *
         * This method returns a span of reflector implementation names that
         * this reflector depends on. The reflection system will ensure that
         * the dependent reflectors are executed before this reflector.
         *
         * @return span of reflector implementation names
         */
        virtual std::span<std::string_view> dependencies() const
        {
            return {};
        }
    };

} // namespace mge::reflection