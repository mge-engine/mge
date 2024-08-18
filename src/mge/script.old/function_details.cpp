// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function_details.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#include <map>
#include <variant>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    class function_dictionary
    {
    public:
        using function_key_type = std::variant<std::string, void*>;

        function_dictionary() = default;
        ~function_dictionary() = default;

        function_details_ref
        create_details(const std::string&                  name,
                       void*                               fptr,
                       const mge::script::invoke_function& function,
                       const std::type_index               return_type,
                       std::vector<std::type_index>&&      argument_types)
        {
            function_key_type k(fptr);
            if (fptr == nullptr) {
                k = name;
            }

            auto it = m_functions.find(k);
            if (it != m_functions.end()) {
                MGE_THROW(illegal_state) << "Function '" << name
                                         << "' already registered under name '"
                                         << it->second->name() << "'";
            }

            auto result =
                std::make_shared<function_details>(name,
                                                   fptr,
                                                   function,
                                                   return_type,
                                                   std::move(argument_types));
            m_functions[k] = result;
            return result;
        }

        std::map<function_key_type, function_details_ref> m_functions;
    };

    static mge::singleton<function_dictionary> s_function_dictionary;

    function_details::function_details(
        const std::string&                  name,
        void*                               fptr,
        const mge::script::invoke_function& function,
        const std::type_index&              return_type,
        std::vector<std::type_index>&&      argument_types)
        : m_name(name)
        , m_fptr(fptr)
        , m_invoke_function(function)
        , m_return_type(return_type)
        , m_argument_types(std::move(argument_types))
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Create function '" << name << "'";
    }

    function_details::~function_details() {}

    const std::string& function_details::name() const { return m_name; }

    const mge::script::invoke_function&
    function_details::invoke_function() const
    {
        return m_invoke_function;
    }

    const std::type_index& function_details::return_type() const
    {
        return m_return_type;
    }

    const std::vector<std::type_index>& function_details::argument_types() const
    {
        return m_argument_types;
    }

    void function_details::set_module(const module_details_ref& m)
    {
        m_module = m;
    }

    function_details_ref function_details::create_details(
        const std::string&                  name,
        void*                               fptr,
        const mge::script::invoke_function& function,
        const std::type_index&              return_type,
        std::vector<std::type_index>&&      argument_types)
    {
        return s_function_dictionary->create_details(name,
                                                     fptr,
                                                     function,
                                                     return_type,
                                                     std::move(argument_types));
    }

} // namespace mge::script