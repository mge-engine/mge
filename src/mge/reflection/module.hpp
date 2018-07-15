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
         * @brief A module represents a C++ name space.
         */
        class MGE_REFLECTION_EXPORT module
        {
        public:
            /**
             * Constructor.
             */
            module();
            /**
             * @brief Construct module with given name.
             * @param name module name
             */
            module(const std::string& name);

            /**
             * @brief Construct module with given name.
             * @param name module name
             */
            module(const char *name);

            /**
             * @brief The global module representing the global name space.
             */
            static module global_module();
        private:
            identifier m_module_id;
        };
    }
}
