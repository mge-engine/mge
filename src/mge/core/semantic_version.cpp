// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/semantic_version.hpp"
#include "boost/boost_spirit.hpp"
#include "mge/core/stdexceptions.hpp"
#include <cctype>
#include <charconv>
#include <iostream>

namespace qi = boost::spirit::qi;

namespace mge {
    semantic_version::semantic_version() {}

    semantic_version::semantic_version(uint32_t major,
                                       uint32_t minor,
                                       uint32_t patch)
        : m_data(major, minor, patch)
    {}

    semantic_version::semantic_version(std::string_view version_str)
        : m_data(0, 0, 0)
    {
        auto set_major = [&](unsigned int val) { std::get<0>(m_data) = val; };
        auto set_minor = [&](unsigned int val) { std::get<1>(m_data) = val; };
        auto set_patch = [&](unsigned int val) { std::get<2>(m_data) = val; };

        if (!qi::parse(version_str.begin(),
                       version_str.end(),
                       qi::uint_[set_major] >>
                           (-('.' >> qi::uint_[set_minor] >>
                              (-('.' >> qi::uint_[set_patch])))) >>
                           qi::eoi)) {
            MGE_THROW(illegal_argument)
                << "Invalid version string " << version_str;
        }
    }

    semantic_version::semantic_version(const semantic_version& v)
        : m_data(v.m_data)
    {}

    semantic_version::semantic_version(semantic_version&& v)
        : m_data(std::move(v.m_data))
    {}

    semantic_version& semantic_version::operator=(const semantic_version& v)
    {
        m_data = v.m_data;
        return *this;
    }

    semantic_version& semantic_version::operator=(semantic_version&& v)
    {
        m_data = std::move(v.m_data);
        return *this;
    }

    bool semantic_version::empty() const noexcept
    {
        decltype(m_data) empty_data;
        return empty_data == m_data;
    }

    bool semantic_version::operator<(const semantic_version& v) const noexcept
    {
        return m_data < v.m_data;
    }

    bool semantic_version::operator==(const semantic_version& v) const noexcept
    {
        return m_data == v.m_data;
    }

    uint32_t semantic_version::major() const noexcept
    {
        return std::get<0>(m_data);
    }

    uint32_t semantic_version::minor() const noexcept
    {
        return std::get<1>(m_data);
    }

    uint32_t semantic_version::patch() const noexcept
    {
        return std::get<2>(m_data);
    }

    std::ostream& operator<<(std::ostream& os, const semantic_version& v)
    {
        if (v.patch() == 0) {
            if (v.minor() == 0) {
                os << v.major();
            } else {
                os << v.major() << "." << v.minor();
            }
        } else {
            return os << v.major() << "." << v.minor() << "." << v.patch();
        }
        return os;
    }

} // namespace mge