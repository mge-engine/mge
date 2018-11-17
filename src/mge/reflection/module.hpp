// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/core/type_name.hpp"
#include <string>
#include <map>

namespace mge {
    namespace reflection {

        /**
         * @brief A module represents a C++ name space.
         */
        class MGE_REFLECTION_EXPORT module
                : public std::enable_shared_from_this<module>
        {
        public:
            /**
             * Constructor. Called to init the global module.
             */
            module();

            /**
             * Constructor. Calles by @c get functions.
             * @param name
             */
            module(const std::string& name);

            /**
             * Get a module by global name.
             * @param name name, which may have scopes
             * @return module that has the global name @c name
             */
            static module& get(const std::string& name);

            /**
             * Get the module that contains the type of
             * the template argument.
             * @return module that contains the template
             *   argument
             */
            template <typename T>
            static module& get()
            {
                return get(namespace_name<T>());
            }

            /**
             * Get the module name. The module name is unique
             * within the containing module.
             *
             * @return module name
             */
            const std::string& name() const
            {
                return m_module_name;
            }

            const std::string global_name() const
            {
                if(m_parent) {
                    return m_parent->global_name() + "::" + m_module_name;
                } else {
                    return "";
                }
            }

            /**
             * @brief The global module representing the global name space.
             */
            static module& global_module();
        private:
            void set_parent(const module_ref& parent);

            module_ref get_or_add(const std::string& local_name);

            std::string m_module_name;
            module_ref m_parent;
            std::map<std::string, module_ref> m_modules;
        };
    }
}
