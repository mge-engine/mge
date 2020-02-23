#include "mge/shader/location.hpp"
namespace mge {
namespace shader {

    location::location()
        :m_module(nullptr)
        ,m_begin(0u)
        ,m_end(0)
    {}

    location::location(const mge::shader::module *m)
        :m_module(m)
        ,m_begin(0u)
        ,m_end(0)
    {}


    location::location(const mge::shader::module *m, uint32_t begin, uint32_t end)
        :m_module(m)
        ,m_begin(begin)
        ,m_end(end)
    {}


    location::location(const location &l)
        :m_module(l.module())
        ,m_begin(l.begin())
        ,m_end(l.end())
    {}


    location::~location()
    {}

    location&
    location::operator =(const location& l)
    {
        m_module = l.m_module;
        m_begin = l.m_begin;
        m_end = l.m_end;
        return *this;
    }

}
}
