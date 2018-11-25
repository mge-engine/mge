#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/function_signature.hpp"
#include "mge/reflection/visitor.hpp"

#include <map>

namespace mge {
    namespace reflection {
        /**
         * A type definition holds all run-time information
         * of a type.
         *
         */
        class MGE_REFLECTION_EXPORT type_definition
        {
        public:
            type_definition(const std::string& name,
                            std::type_index index,
                            size_t size);
            ~type_definition() = default;

            const std::string& name() const { return m_name; }
            size_t size() const { return m_size; }

            void apply(visitor& v) const;
        private:
            std::string     m_name;
            std::type_index m_type_index;
            size_t          m_size;
            std::map<std::string, int64_t> m_enum_values;
        };
    }
}
