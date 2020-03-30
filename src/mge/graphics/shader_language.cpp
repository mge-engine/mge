// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_language.hpp"
#include "mge/core/stdexceptions.hpp"
#include <boost/algorithm/string.hpp>
#include <vector>

namespace mge {
    shader_language::shader_language() {}

    shader_language::shader_language(const std::string & t,
                                     const mge::version &v)
        : m_type(t), m_version(v)
    {}

    shader_language::shader_language(const std::string &l) { from_string(l); }

    shader_language::shader_language(const char *l)
    {
        from_string(std::string(l));
    }

    void shader_language::from_string(const std::string &s)
    {
        auto                     trimmed = boost::trim_copy(s);
        std::vector<std::string> components;
        boost::split(components, trimmed, boost::is_any_of(" "));
        if (components.empty() || components.size() > 2) {
            MGE_THROW(mge::illegal_argument)
                << "Invalid shader language '" << s << "'";
        }
        m_type = boost::to_lower_copy(boost::trim_copy(components[0]));
        if (components.size() == 2) {
            m_version = mge::version(components[1]);
        }
    }

    bool shader_language::operator<(const shader_language &l) const
    {
        if (type() < l.type()) {
            return true;
        } else if (type() == l.type()) {
            return version() < l.version();
        }
        return false;
    }

    bool shader_language::operator==(const shader_language &l) const
    {
        return type() == l.type() && version() == l.version();
    }

    std::ostream &operator<<(std::ostream &os, const shader_language &l)
    {
        os << l.type();
        if (!l.version().empty()) {
            os << " " << l.version();
        }
        return os;
    }

} // namespace mge
