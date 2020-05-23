// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/version.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/lexical_cast.hpp"

#include <vector>

namespace mge {

    version::version() : m_data(0u, 0u, 0u) {}

    version::version(unsigned int major, unsigned int minor, unsigned int patch)
        : m_data(major, minor, patch)
    {}

    version::version(const version &v) : m_data(v.m_data) {}

    version::version(version &&v) : m_data(std::move(v.m_data)) {}

    version::version(const std::string &version) { from_string(version); }

    version::version(const char *version) { from_string(std::string(version)); }

    version &version::operator=(const version &v)
    {
        m_data = v.m_data;
        return *this;
    }

    bool version::empty() const noexcept
    {
        version v;
        return *this == v;
    }

    version &version::operator=(version &&v)
    {
        m_data = std::move(v.m_data);
        return *this;
    }

    bool version::operator<(const version &v) const noexcept
    {
        return m_data < v.m_data;
    }

    bool version::operator==(const version &v) const noexcept
    {
        return m_data == v.m_data;
    }

    void version::from_string(const std::string &s)
    {
        auto                     trimmed = boost::trim_copy(s);
        std::vector<std::string> components;
        boost::split(components, trimmed, boost::is_any_of("."));
        if (components.size() > 3 || components.empty()) {
            MGE_THROW(mge::illegal_argument)
                << "Invalid version string '" << s << "'";
        }
        try {
            unsigned int major = lexical_cast<unsigned int>(components[0]);
            unsigned int minor = 0;
            unsigned int patch = 0;
            if (components.size() > 1) {
                minor = lexical_cast<unsigned int>(components[1]);
                if (components.size() > 2) {
                    patch = lexical_cast<unsigned int>(components[2]);
                }
            }
            m_data = std::make_tuple(major, minor, patch);
        } catch (const mge::bad_cast &) {
            MGE_THROW(mge::illegal_argument)
                << "Invalid version string '" << s << "'";
        }
    }

    std::ostream &operator<<(std::ostream &os, const version &v)
    {
        unsigned int major_ = v.major();
        unsigned int minor_ = v.minor();
        unsigned int patch_ = v.patch();

        if (minor_ == 0 && patch_ == 0) {
            return os << major_;
        } else if (minor_ != 0 && patch_ == 0) {
            return os << major_ << "." << minor_;
        } else {
            return os << major_ << "." << minor_ << "." << patch_;
        }
    }

} // namespace mge
