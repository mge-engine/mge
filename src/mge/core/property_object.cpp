// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
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

    bool
    property_object::exists_property(const std::string& key) const
    {
        return m_properties.exists(key);
    }


    void
    property_object::erase_property(const std::string& key)
    {
        m_properties.erase(key);
    }


    void
    property_object::clear_properties()
    {
        m_properties.clear();
    }


}
