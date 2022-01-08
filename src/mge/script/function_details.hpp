// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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
        function_details(const std::string& name);
        virtual ~function_details();

        const std::string& name() const;

        static function_details_ref
        create_details(const std::string&                  name,
                       void*                               fptr,
                       const mge::script::invoke_function& function,
                       const std::type_index               return_type,
                       std::vector<std::type_index>&&      argument_types);

    private:
        std::string m_name;
    };

} // namespace mge::script