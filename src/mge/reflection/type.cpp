#include "mge/reflection/type.hpp"

namespace mge {
    namespace reflection {

        type::type(const std::string& name,
                   std::type_index index,
                   size_t size,
                   bool is_enum,
                   bool is_pod)
            :m_name(name)
            ,m_type_index(index)
            ,m_size(size)
            ,m_is_enum(is_enum)
            ,m_is_pod(is_pod)
        {}

        void
        type::apply(visitor& v) const
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
