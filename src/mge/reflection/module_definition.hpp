// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"
#include <string>

namespace mge {
    namespace reflection {
        /**
         * @brief Definition of a module.
         */
        class MGE_REFLECTION_EXPORT module_definition
        {
        public:
            /**
             * Construct named module.
             * @param name module name
             */
            module_definition(const std::string& name);

            /**
             * Construct global module.
             */
            module_definition();

            /**
             * Destructor.
             */
            ~module_definition();
        };
    }
}
