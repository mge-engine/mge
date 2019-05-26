#include "mge/core/property_object.hpp"

namespace mge {
    std::string
    property_object::get_property(const std::string &key) const
    {
        return m_properties.get<std::string>(key);
    }

    std::string
    property_object::get_property(const std::string &key, const std::string& default_value) const
    {
        return m_properties.get<std::string, std::string>(key, default_value);
    }

}
