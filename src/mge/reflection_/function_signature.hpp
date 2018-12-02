#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/signature.hpp"
namespace mge {
    namespace reflection {


        class MGE_REFLECTION_EXPORT function_signature
        {
        public:
            function_signature();
            function_signature(const std::type_index& retval_type,
                               const signature& parameter_types);
            function_signature(const function_signature& s);
            function_signature(function_signature&& s);

            ~function_signature() = default;

            function_signature& operator =(const function_signature& s);
            function_signature& operator =(function_signature&& s);

            const signature& parameter_types() const;
            const std::type_index& return_value_type() const;
        private:
            std::type_index m_return_value_type;
            signature       m_parameter_types;
        };

        MGE_REFLECTION_EXPORT
        std::ostream& operator <<(std::ostream& os, const function_signature& s);
    }
}
