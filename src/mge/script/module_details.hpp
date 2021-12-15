// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace mge::script {

    class MGESCRIPT_EXPORT module_details
        : public std::enable_shared_from_this<::mge::script::module_details>
    {
    public:
        module_details(const module_details_ref& parent,
                       const std::string&        name);
        ~module_details();

        const std::string& name() const;
        std::string full_name() const;
        bool is_root() const;

        module_details_ref parent() const;

        static module_details_ref root();

        module_details_ref get_or_add_child(const std::string& name);

        void add_child(const module_details_ref& child);
        void add_type(const type_details_ref& child);
        void add_variable(const variable_details_ref& child);
        void add_function(const function_details_ref& child);

        void apply(visitor& v);

    private:
        module_details_weak_ref                     m_parent;
        std::map<std::string, type_details_ref>     m_types;
        std::map<std::string, variable_details_ref> m_variables;
        std::map<std::string, function_details_ref> m_functions;
        std::map<std::string, module_details_ref>   m_children;
        const std::string                           m_name;
    };

} // namespace mge::script