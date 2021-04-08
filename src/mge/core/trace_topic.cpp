// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_topic.hpp"
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/memory_trace_sink.hpp"
#include "mge/core/simple_trace_formatter.hpp"
#include "mge/core/stream_trace_sink.hpp"

#include <iostream>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace mge {

    MGE_DEFINE_TRACE(MGE);
    MGE_DEFINE_TRACE(CORE);

    MGE_DEFINE_PARAMETER(bool, trace, print_to_stdout,
                         "Whether all trace shall be printed to stdout");

    static volatile bool global_topic_configured;

    static bool global_print_to_stdout()
    {
        if (MGE_PARAMETER(trace, print_to_stdout).get(false)) {
            return true;
        }

        char *val = ::getenv("MGE_TRACE_TO_STDOUT");
        if (val) {
            return true;
        } else {
            return false;
        }
    }

    static bool global_trace_enabled()
    {
        char *val = ::getenv("MGE_TRACE_ENABLED");
        if (val) {
            return true;
        } else {
            return false;
        }
    }

    trace_topic::~trace_topic() { m_level_config.set_change_handler(nullptr); }

    trace_level level_from_string(const std::string &s)
    {
        if (s == "DEBUG"s) {
            return trace_level::DEBUG;
        } else if (s == "ERROR"s) {
            return trace_level::ERROR;
        } else if (s == "INFO"s) {
            return trace_level::INFO;
        } else if (s == "WARNING"s) {
            return trace_level::WARNING;
        } else if (s == "FATAL"s) {
            return trace_level::FATAL;
        } else if (s == "ALL"s) {
            return trace_level::ALL;
        }
        return trace_level::NONE;
    }

    void trace_topic::initialize()
    {
        if (m_topic == "MGE"sv) {
            m_sinks.push_back(std::make_shared<memory_trace_sink>());
            enable(trace_level::ALL);
        }
    }

    void trace_topic::configure()
    {
        if (configuration::loaded()) {
            update_configuration();
        }
    }

    void trace_topic::update_configuration()
    {
        disable(trace_level::ALL);
        if (m_level_config.has_value()) {
            std::string              level_list_str = m_level_config.get();
            std::vector<std::string> level_list;
            boost::split(level_list, level_list_str, boost::is_any_of(","),
                         boost::token_compress_on);
            std::for_each(level_list.begin(), level_list.end(),
                          [&](std::string &s) {
                              boost::to_upper(s);
                              boost::trim(s);
                              trace_level l = level_from_string(s);
                              if (l != trace_level::NONE) {
                                  this->enable(l);
                              }
                          });
        }
        if (m_topic == "MGE"sv) {
            if (global_trace_enabled()) {
                this->enable(trace_level::ALL);
            }
            if (!global_topic_configured) {
                if (global_print_to_stdout()) {
                    trace_sink_ref old;
                    if (m_sinks.size() == 1) {
                        old = m_sinks[0];
                        m_sinks.clear();
                    }

                    auto fmt = std::make_shared<simple_trace_formatter>();
                    auto sink =
                        std::make_shared<stream_trace_sink>(std::cout, fmt);
                    m_sinks.push_back(sink);

                    if (old) {
                        memory_trace_sink *s =
                            dynamic_cast<memory_trace_sink *>(old.get());
                        s->forward(*sink);
                    }
                }
                global_topic_configured = true;
            }
        }
    }

    bool trace_topic::enabled(trace_level l) const noexcept
    {

        uint8_t l_int = static_cast<uint8_t>(l);
        if ((l_int & m_enabled_levels)) {
            return true;
        } else {
            if (this != &__trace_topic_MGE()) {
                return __trace_topic_MGE().enabled(l);
            } else {
                return false;
            }
        }
    }

    void trace_topic::enable(trace_level l)
    {
        uint8_t l_int = static_cast<uint8_t>(l);
        m_enabled_levels |= l_int;
    }

    void trace_topic::disable(trace_level l)
    {
        uint8_t l_int_inverted = ~static_cast<uint8_t>(l);
        m_enabled_levels &= l_int_inverted;
    }

    void trace_topic::set_level(trace_level l)
    {
        m_enabled_levels = static_cast<uint8_t>(l);
    }

    void trace_topic::set_level(uint8_t l)
    {
        uint8_t l_all    = static_cast<uint8_t>(trace_level::ALL);
        m_enabled_levels = l & l_all;
    }

    std::string_view trace_topic::name() const noexcept { return m_topic; }

    bool trace_topic::global() const noexcept
    {
        return this == &__trace_topic_MGE();
    }

    void trace_topic::publish(const trace_record &r)
    {
        for (auto &s : m_sinks) {
            s->publish(r);
        }

        for (auto &s : __trace_topic_MGE().m_sinks) {
            s->publish(r);
        }
    }

    void trace_topic::add_sink(const std::shared_ptr<trace_sink> &sink)
    {
        m_sinks.emplace_back(sink);
    }

    void trace_topic::remove_sink(const std::shared_ptr<trace_sink> &sink)
    {
        for (sink_vector::iterator i = m_sinks.begin(); i != m_sinks.end();
             ++i) {
            if (&(*sink) == &(*(*i))) {
                m_sinks.erase(i);
                break;
            }
        }
    }
} // namespace mge