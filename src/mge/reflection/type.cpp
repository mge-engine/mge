#include "mge/reflection/type.hpp"

namespace mge {
    namespace reflection {

        type::type(const std::string& name,
                   std::type_index index,
                   bool is_enum)
            :m_name(name)
            ,m_type_index(index)
            ,m_is_enum(is_enum)
        {}

        void
        type::apply(visitor& v)
        {
            v.start(*this);
            if(m_is_enum) {
                for(const auto &e : m_enum_values) {
                    v.enum_value(e.first, e.second);
                }
            }
            v.finish(*this);
        }

    }
}
