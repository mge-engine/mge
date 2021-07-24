// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_language.hpp"
#include <ostream>

namespace mge {

    shader_language::shader_language() {}

    shader_language::shader_language(std::string_view        name,
                                     const semantic_version &version)
        : m_name(name), m_version(version)
    {}

    shader_language::~shader_language() {}

    std::string_view shader_language::name() const { return m_name; }

    const semantic_version &shader_language::version() const
    {
        return m_version;
    }

    bool shader_language::operator==(const shader_language &l) const
    {
        return m_name == l.m_name && m_version == l.m_version;
    }

    bool shader_language::compatible(const shader_language &other) const
    {
        if (m_name == other.m_name) {
            return m_version <= other.m_version;
        } else {
            return false;
        }
    }

    std::ostream &operator<<(std::ostream &os, const shader_language &l)
    {
        return os << l.name() << " " << l.version();
    }
} // namespace mge