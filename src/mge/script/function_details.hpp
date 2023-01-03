// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
#include <typeindex>
#include <vector>

namespace mge::script {

    class MGESCRIPT_EXPORT function_details
    {
    public:
        function_details(const std::string&                  name,
                         void*                               fptr,
                         const mge::script::invoke_function& function,
                         const std::type_index&              return_type,
                         std::vector<std::type_index>&&      argument_types);
        virtual ~function_details();

        const std::string&                  name() const;
        const mge::script::invoke_function& invoke_function() const;
        const std::type_index&              return_type() const;
        const std::vector<std::type_index>& argument_types() const;
        void set_module(const module_details_ref& m);

        static function_details_ref
        create_details(const std::string&                  name,
                       void*                               fptr,
                       const mge::script::invoke_function& function,
                       const std::type_index&              return_type,
                       std::vector<std::type_index>&&      argument_types);

    private:
        std::string                  m_name;
        void*                        m_fptr;
        mge::script::invoke_function m_invoke_function;
        std::type_index              m_return_type;
        std::vector<std::type_index> m_argument_types;
        module_details_weak_ref      m_module;
    };

} // namespace mge::script