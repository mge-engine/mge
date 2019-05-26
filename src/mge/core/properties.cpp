#include "mge/core/properties.hpp"

namespace mge {
    properties::properties()
    {}

    properties::properties(const properties& p)
        :m_data(p.m_data)
    {}

    properties::properties(properties&& p)
        :m_data(std::move(p.m_data))
    {}

    properties&
    properties::operator =(const properties& p)
    {
        m_data = p.m_data;
        return *this;
    }

    properties&
    properties::operator =(properties&& p)
    {
        m_data = std::move(p.m_data);
        return *this;
    }

    void
    properties::clear()
    {
        m_data.clear();
    }

    void
    properties::erase(const char *key)
    {
        m_data.erase(key);
    }

    void
    properties::erase(const std::string& key)
    {
        m_data.erase(key);
    }

}
