// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/script/dllexport.hpp"
#include "mge/script/module.hpp"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace mge::script {

    /**
     * @brief Detail information of module.
     */
    class MGESCRIPT_EXPORT module_details
        : public std::enable_shared_from_this<module_details>
    {
    public:
        /**
         * @brief Constructor.
         *
         * @param name local module name
         */
        module_details(const std::string& name);

        /**
         * @brief Construct root module details.
         */
        module_details();

        /**
         * @brief Destructor.
         */
        ~module_details();

        /**
         * @brief Get instance by name.
         *
         * @param path module name
         * @return module details instance
         */
        static module_details_ref get(const std::string& path);

        /**
         * @brief Return whether the module is the root module.
         *
         * @return @c true if the module is the root module
         */
        bool is_root() const { return m_parent.expired(); }

        /**
         * @brief Retrieve parent module.
         *
         * @return parent module, nor null reference if this is the root module
         */
        module_details_ref parent() const { return m_parent.lock(); }

        /**
         * @brief Module name.
         *
         * @return name of the module
         */
        const std::string& name() const { return m_name; }

        /**
         * @brief Full module name.
         *
         * @return Full module name, components separated by "::"
         */
        std::string full_name() const;

        /**
         * @brief Applies a module visitor.
         *
         * @param v visitor
         */
        void apply(visitor& v) const;

        /**
         * @brief Add a sub-module.
         *
         * @param m sub module
         */
        void add_module(module_details_ref& m);

        /**
         * @brief Add type to module.
         *
         * @param t type
         */
        void add_type(type_base& t);

        /**
         * @brief Add a type to the module.
         *
         * @param t type
         */
        void add_type(type_details_ref& t);

        /**
         * @brief Add a function to the module.
         *
         * @param f function
         */
        void add_function(function_base& f);

        /**
         * @brief Add a function to the module.
         *
         * @param f function
         */
        void add_function(function_details_ref& f);

    private:
        module_details_ref get_or_add_module(const std::string& name);

        using module_map = std::unordered_map<std::string, module_details_ref>;
        using type_map = std::unordered_map<std::type_index, type_details_ref>;
        using function_map =
            std::unordered_map<std::string, function_details_ref>;

        std::string             m_name;
        module_details_weak_ref m_parent;
        module_map              m_modules;
        type_map                m_types;
        function_map            m_functions;
    };

} // namespace mge::script