// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"
#include "mge/core/sequence.hpp"
#include "mge/core/singleton.hpp"
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
            module_definition(const char* name);

            /**
             * Construct global module.
             */
            module_definition();

            /**
             * Destructor.
             */
            ~module_definition();

            /**
             * @brief Get module identifier.
             * @return identifier
             */
            identifier id() const;

            /**
             * Module definition name.
             * @return name
             */
            const std::string& name() const;

            /**
             * @brief Get module definition by name.
             * @param name module name
             * @return module definition
             */
            static module_definition& get(const char *name);
        private:
            std::string m_name;
            identifier  m_id;
            static sequence<identifier> s_sequence;
        };
    }
}
