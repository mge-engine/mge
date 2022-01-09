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
         * @param name module name
         * @return module details instance
         */
        static module_details_ref get(const std::string& path);

        bool               is_root() const { return m_parent.expired(); }
        module_details_ref parent() const { return m_parent.lock(); }
        const std::string& name() const { return m_name; }
        std::string        full_name() const;

        void apply(visitor& v);

        void add_module(module_details_ref& m);
        void add_type(type_base& t);
        void add_type(type_details_ref& t);
        void add_function(function_base& f);
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