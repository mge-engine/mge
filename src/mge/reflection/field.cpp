#include "mge/reflection/field.hpp"
#include "mge/reflection/type.hpp"
#include "mge/reflection/type_definition.hpp"
namespace mge {
    namespace reflection {
        field::field()
            :m_name("")
            ,m_type()
        {
            m_type = mge::reflection::type<void>().definition();
        }

        field::field(const char *name, const type_definition_ref& type)
            :m_name(name)
            ,m_type(type)
        {}

        field::field(const field& f)
            :m_name(f.m_name)
            ,m_type(f.m_type)
        {}

        field&
        field::operator =(const field& f)
        {
            m_name = f.m_name;
            m_type = f.m_type;
            return *this;
        }
    }
}
