#include "mge/reflection/type_definition.hpp"
namespace mge {
    namespace reflection {

        type_definition::type_definition(const std::string &name,
                                         std::type_index index,
                                         size_t size,
                                         bool is_enum)
            :m_name(name)
            ,m_type_index(index)
            ,m_size(size)
            ,m_is_enum(is_enum)
        {}

        void
        type_definition::apply(visitor& v) const
        {
            v.start(*this);
            if(m_is_enum) {
                for(const auto& it : m_enum_values) {
                    v.enum_value(it.first,
                                 it.second);
                }
            }
            v.finish(*this);
        }

        void
        type_definition::enum_value(const char *name, int64_t value)
        {
            m_enum_values[name] = value;
        }

        void
        type_definition::constructor(const signature &s,
                                     const constructor::function &f)
        {
            m_constructors[s] = mge::reflection::constructor(s, f);
        }

        void
        type_definition::destructor(const destructor::function& df)
        {
            m_destructor = mge::reflection::destructor(df);
        }

        void
        type_definition::field(const char *name,
                               const type_definition_ref& type)
        {
            m_fields.emplace_back(name, type);
        }

    }
}
