// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/parameter.hpp"
#include "mge/core/configuration.hpp"

namespace mge {
    basic_parameter::basic_parameter(std::string_view section,
                                     std::string_view name,
                                     std::string_view description)
        : m_section(section), m_name(name), m_description(description)
    {
        configuration::register_parameter(*this);
    }

    basic_parameter::~basic_parameter()
    {
        configuration::unregister_parameter(*this);
    }

    std::string_view basic_parameter::section() const noexcept
    {
        return m_section;
    }
    std::string_view basic_parameter::name() const noexcept { return m_name; }
    std::string_view basic_parameter::description() const noexcept
    {
        return m_description;
    }
} // namespace mge