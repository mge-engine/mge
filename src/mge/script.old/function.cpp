// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/function_details.hpp"

namespace mge::script {

    const std::string& function_base::name() const { return m_details->name(); }

    const mge::script::invoke_function& function_base::invoke_function() const
    {
        return m_details->invoke_function();
    }

    const std::type_index& function_base::return_type() const
    {
        return m_details->return_type();
    }

    const std::vector<std::type_index>& function_base::argument_types() const
    {
        return m_details->argument_types();
    }

    const function_details_ref& function_base::details() const
    {
        return m_details;
    }

    function_details_ref& function_base::details() { return m_details; }

    function_details_ref
    function_base::create_details(const std::string&                  name,
                                  void*                               fptr,
                                  const mge::script::invoke_function& function,
                                  const std::type_index& return_type)
    {
        std::vector<std::type_index> argument_types_v;
        return create_details(name,
                              fptr,
                              function,
                              return_type,
                              std::move(argument_types_v));
    }

    function_details_ref
    function_base::create_details(const std::string&                  name,
                                  void*                               fptr,
                                  const mge::script::invoke_function& function,
                                  const std::type_index&         return_type,
                                  std::vector<std::type_index>&& argument_types)
    {
        return function_details::create_details(name,
                                                fptr,
                                                function,
                                                return_type,
                                                std::move(argument_types));
    }

} // namespace mge::script