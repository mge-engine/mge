// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/parameter.hpp"
#include "mge/core/configuration.hpp"

namespace mge {
    basic_parameter::basic_parameter(std::string_view section,
                                     std::string_view name,
                                     std::string_view description)
        : m_description(description)
    {
        m_path /= section;
        m_path /= name;
        configuration::register_parameter(*this);
    }

    basic_parameter::~basic_parameter()
    {
        configuration::unregister_parameter(*this);
    }

    const mge::path& basic_parameter::path() const
    {
        return m_path;
    }

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
        const basic_parameter::change_callback& callback)
    {
        std::lock_guard<std::mutex> guard(m_change_lock);
        m_change_callback = callback;
    }

    void basic_parameter::read_value(const mge::json::json& doc)
    {
        if (m_read_function) {
            m_read_function(doc);
        }
    }

    void basic_parameter::write_value(mge::json::json& doc) const
    {
        if (m_write_function) {
            m_write_function(doc);
        }
    }

    void basic_parameter::set_value(const std::any& value)
    {
        if (m_set_function) {
            m_set_function(value);
        }
    }

    void basic_parameter::reset() {}
} // namespace mge