#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include <string>
namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT field
        {
        public:
            field();
            field(const char *name,
                  const type_definition_ref& type);
            field(const field& f);
            ~field() = default;

            field& operator =(const field& f);

            const std::string& name() const { return m_name; }
            const type_definition_ref& type() const { return m_type; }
        private:
            std::string         m_name;
            type_definition_ref m_type;
        };
    }
}
