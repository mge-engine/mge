// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_profile.hpp"
#include <ostream>

namespace mge {

    shader_profile::shader_profile(std::string_view        name,
                                   const semantic_version& version)
        : m_name(name)
        , m_version(version)
    {}

    std::string_view shader_profile::name() const
    {
        return m_name;
    }

    const semantic_version& shader_profile::version() const
    {
        return m_version;
    }

    bool shader_profile::operator==(const shader_profile& p) const
    {
        return m_name == p.m_name && m_version == p.m_version;
    }

    bool shader_profile::operator!=(const shader_profile& p) const
    {
        return m_name != p.m_name || m_version != p.m_version;
    }

    bool shader_profile::compatible(const shader_profile& other) const
    {
        if (m_name == other.m_name) {
            return m_version <= other.m_version;
        } else {
            return false;
        }
    }

    std::ostream& operator<<(std::ostream& os, const shader_profile& prof)
    {
        os << prof.name() << " " << prof.version();
        return os;
    }
} // namespace mge
