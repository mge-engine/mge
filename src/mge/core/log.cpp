// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/log.hpp"
#include "mge/config.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/log_sink.hpp"
#include "mge/core/singleton.hpp"

MGE_DEFINE_LOG(MGE)

namespace mge {

    static inline uint8_t severity_from_string(const std::string &value)
    {
        if (value == "ALL") {
            return (uint8_t)log_severity::ALL;
        } else {
            return (uint8_t)log_severity::NONE;
        }
    }

    class log_sink_manager
    {
    public:
        using sink_map_t = std::map<std::string, log_sink_ref>;

        log_sink_manager()
            : m_configured(false), m_global_level((uint8_t)log_severity::ALL),
              m_config_version(0)
        {}

        void publish(const log_record &r)
        {
            if (!m_configured) {
                configure();
            }
            // no tracing until configuring works
            if (!m_configured) {
                return;
            }
            for (const auto &p : m_sinks) {
                p.second->publish(r);
            }
        }

        uint8_t configured_severity(const char *topic) const
        {
            auto it = m_levels.find(topic);
            if (it != m_levels.end()) {
                return it->second | m_global_level;
            } else {
                return m_global_level;
            }
        }

        uint32_t version() const { return m_config_version; }

    private:
        void store_default_config(configuration &config)
        {
            std::string logfile_name(executable_name());
            logfile_name.append(".txt");

            config.set("sinks", "console");
            config.set("sink.defaultfile.formatter", "text");
            config.set("sink.defaultfile.class", "file");
            config.set("sink.defaultfile.file", logfile_name);
            config.set("sink.console.formatter", "text");
            config.set("sink.console.class", "console");
            config.set("level.ALL", "ALL");
            config.store();
        }

        void configure()
        {
            try {
                configuration config("log");
                if (!config.contains_key("sinks")) {
                    store_default_config(config);
                }

                sink_map_t sinks;
                auto       sink_names = config.list_value("sinks");
                for (const auto &sink_name : sink_names) {
                    std::string config_name("log.sink.");
                    config_name += sink_name;
                    configuration sink_config(config_name);
                    std::string   sink_class = sink_config.value("class");
                    auto          sink       = log_sink::create(sink_class);
                    if (!sink) {
                        return;
                    }
                    sink->configure(sink_config);
                    sinks[sink_name] = sink;
                }
                m_sinks = sinks;
                configuration level_config("log.level");
                auto          keys = level_config.keys();
                for (const auto &k : keys) {
                    m_levels[k] = severity_from_string(level_config.value(k));
                }
                m_global_level = m_levels["ALL"];
                ++m_config_version;
                m_configured = true;
            } catch (const mge::exception &) {
                // ignore failures in lookup
            }
        }

        bool                           m_configured;
        uint8_t                        m_global_level;
        volatile uint32_t              m_config_version;
        sink_map_t                     m_sinks;
        std::map<std::string, uint8_t> m_levels;
    };

    static singleton<log_sink_manager> s_log_sinks;

    log::log(const char *topic) : m_severity_mask(0), m_config_version(0)
    {
        m_log_record.category  = topic;
        m_log_record.severity  = log_severity::NONE;
        m_log_record.thread_id = mge::this_thread::get_id();
        m_log_record.message   = "";
        m_severity_mask        = s_log_sinks->configured_severity(topic);
        m_config_version       = s_log_sinks->version();
    }

    log &log::begin_entry(log_severity severity)
    {
        flush_current_record();
        m_log_record.timestamp = std::chrono::system_clock::now();
        m_log_record.severity  = severity;
        return *this;
    }

    void log::flush_current_record()
    {
        if (m_log_record.severity == log_severity::NONE) {
            return;
        }
        publish(m_log_record.set_message(m_message.str().c_str()));
        m_log_record.set_message("");
        m_log_record.severity = log_severity::NONE;
        m_message.str("");
    }

    void log::publish(const log_record &record)
    {
        s_log_sinks->publish(record);
    }

    bool log::enabled(log_severity s)
    {
        if (s_log_sinks->version() == m_config_version) {
            return (m_severity_mask & (uint8_t)s) != 0;
        } else {
            m_severity_mask =
                s_log_sinks->configured_severity(m_log_record.category);
            m_config_version = s_log_sinks->version();
            return (m_severity_mask & (uint8_t)s) != 0;
        }
    }

} // namespace mge
