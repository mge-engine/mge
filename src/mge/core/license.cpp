// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/license.hpp"
#include "mge/core/stdexceptions.hpp"
#include <iostream>

namespace mge {

    std::ostream &operator<<(std::ostream &os, license_type type)
    {
        switch (type) {
        case license_type::UNKNOWN:
            return os << "UNKNOWN";
        case license_type::MIT:
            return os << "MIT";
        case license_type::BSD:
            return os << "BSD";
        case license_type::GPL2:
            return os << "GPL2";
        case license_type::LGPL2:
            return os << "LGPL2";
        case license_type::GPL3:
            return os << "GPL3";
        case license_type::LGPL3:
            return os << "LGPL3";
        case license_type::PUBLIC_DOMAIN:
            return os << "PUBLIC_DOMAIN";
        case license_type::CC:
            return os << "CC";
        case license_type::COMMERCIAL:
            return os << "COMMERCIAL";
        case license_type::OPEN_SOURCE:
            return os << "OPEN_SOURCE";
        case license_type::OTHER:
            return os << "OTHER";
        default:
            return os << "?";
        }
    }

    license::license() : m_type(license_type::UNKNOWN) {}

    license::license(license_type type) : m_type(type) {}

    license::license(license_type type, std::string_view text,
                     const mge::version &version)
        : m_type(type), m_text(text), m_version(version)
    {}

    license_type license::type() const { return m_type; }

    std::string_view license::text() const { return m_text; }

    mge::version license::version() const { return m_version; }

} // namespace mge
