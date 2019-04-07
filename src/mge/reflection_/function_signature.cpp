// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function_signature.hpp"
namespace mge {
    namespace reflection {
        function_signature::function_signature()
            :m_return_value_type(std::type_index(typeid(void)))
        {}

        function_signature::function_signature(const std::type_index& retval_type,
                                               const signature& parameter_types)
            :m_return_value_type(retval_type)
            ,m_parameter_types(parameter_types)
        {}

        function_signature::function_signature(const function_signature& s)
            :m_return_value_type(s.m_return_value_type)
            ,m_parameter_types(s.m_parameter_types)
        {}

        function_signature::function_signature(function_signature&& s)
            :m_return_value_type(s.m_return_value_type)
            ,m_parameter_types(std::move(s.m_parameter_types))
        {}

        function_signature&
        function_signature::operator =(const function_signature& s)
        {
            m_return_value_type = s.m_return_value_type;
            m_parameter_types = s.m_parameter_types;
            return *this;
        }

        function_signature&
        function_signature::operator =(function_signature&& s)
        {
            m_return_value_type = s.m_return_value_type;
            m_parameter_types = std::move(s.m_parameter_types);
            return *this;
        }

        const signature&
        function_signature::parameter_types() const
        {
            return m_parameter_types;
        }

        const std::type_index&
        function_signature::return_value_type() const
        {
            return m_return_value_type;
        }

        std::ostream&
        operator <<(std::ostream& os, const function_signature& s)
        {
            return os << s.return_value_type().name() << " " << s.parameter_types();
        }
    }
}
