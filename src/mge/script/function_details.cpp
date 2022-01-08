// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_details.hpp"

namespace mge::script {
    function_details::function_details(const std::string& name)
        : m_name(name)
    {}

    function_details::~function_details() {}

    const std::string& function_details::name() const { return m_name; }

    function_details_ref function_details::create_details(
        const std::string&                  name,
        void*                               fptr,
        const mge::script::invoke_function& function,
        const std::type_index               return_type,
        std::vector<std::type_index>&&      argument_types)
    {
        function_details_ref result;
        return result;
    }

} // namespace mge::script