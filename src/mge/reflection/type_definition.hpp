#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/function_signature.hpp"
#include "mge/reflection/visitor.hpp"
#include "mge/reflection/constructor.hpp"

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
                            size_t size,
                            bool is_enum);
            ~type_definition() = default;

            const std::string& name() const { return m_name; }
            size_t size() const { return m_size; }

            void apply(visitor& v) const;

            void enum_value(const char *name, int64_t value);
            void constructor(const signature& signature,
                             const constructor::function& f);
        private:
            std::string     m_name;
            std::type_index m_type_index;
            size_t          m_size;
            bool            m_is_enum;
            std::map<std::string, int64_t> m_enum_values;
            std::map<signature, mge::reflection::constructor> m_constructors;
        };
    }
}
