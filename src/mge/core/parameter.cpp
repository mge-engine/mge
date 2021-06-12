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

    void basic_parameter::notify_change()
    {
        std::lock_guard<std::mutex> guard(m_change_lock);

        if (m_change_callback) {
            m_change_callback();
        }
    }

    basic_parameter::change_callback basic_parameter::change_handler() const
    {
        std::lock_guard<std::mutex> guard(m_change_lock);
        return m_change_callback;
    }

    void basic_parameter::set_change_handler(
        const basic_parameter::change_callback &callback)
    {
        std::lock_guard<std::mutex> guard(m_change_lock);
        m_change_callback = callback;
    }

    void basic_parameter::reset() {}

} // namespace mge