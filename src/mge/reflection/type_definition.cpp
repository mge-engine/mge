#include "mge/reflection/type_definition.hpp"
namespace mge {
    namespace reflection {

        type_definition::type_definition(const std::string &name,
                                         std::type_index index,
                                         size_t size)
            :m_name(name)
            ,m_type_index(index)
            ,m_size(size)
        {}

        void
        type_definition::apply(visitor& v) const
        {
            v.start(*this);
            v.finish(*this);
        }

    }
}
