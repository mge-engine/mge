// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/log.hpp"
#include "mge/core/log_sink.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/configuration.hpp"

namespace mge {

    class log_sink_manager
    {
    public:
        typedef std::map<std::string, log_sink_ref> sink_map_t;

        log_sink_manager()
            :m_configured(false)
        {}

        void publish(const log_record& r)
        {
            if(!m_configured) {
                configure();
            }
            // no tracing until configuring works
            if(!m_configured) {
                return;
            }
            for(const auto& p: m_sinks) {
                p.second->publish(r);
            }
        }
    private:
        void store_default_config(configuration& config)
        {
            std::string logfile_name(executable_name());
            logfile_name.append(".txt");

            config.set("sinks", "console");
            config.set("sink.defaultfile.formatter", "text");
            config.set("sink.defaultfile.class", "file");
            config.set("sink.defaultfile.file", logfile_name);
            config.set("sink.console.formatter", "text");
            config.set("sink.console.class", "console");

            config.store();
        }

        void configure()
        {
            try {
                configuration config("log");
                if(!config.contains_key("sinks")) {
                    store_default_config(config);
                }

                sink_map_t sinks;
                auto sink_names = config.list_value("sinks");
                for(const auto& sink_name : sink_names) {
                    std::string config_name("log.sink.");
                    config_name += sink_name;
                    configuration sink_config(config_name);
                    std::string sink_class = sink_config.value("class");
                    auto sink = log_sink::create(sink_class);
                    if(!sink) {
                        return;
                    }
                    sink->configure(sink_config);
                    sinks[sink_name] = sink;
                }
                m_sinks = sinks;
                m_configured = true;
            } catch(const mge::exception& ex) {
                // ignore failures in lookup
            }
        }

        bool       m_configured;
        sink_map_t m_sinks;
    };

    static singleton<log_sink_manager> s_log_sinks;

    log::log(const char *topic)
    {
        m_log_record.category  = topic;
        m_log_record.severity  = log_severity::NONE;
        m_log_record.thread_id = mge::this_thread::get_id();
        m_log_record.message = "";
    }

    log&
    log::begin_entry(log_severity severity)
    {
        flush_current_record();
        m_log_record.timestamp = std::chrono::system_clock::now();
        m_log_record.severity  = severity;
        return *this;
    }

    void
    log::flush_current_record()
    {
        if(m_log_record.severity == log_severity::NONE) {
            return;
        }
        publish(m_log_record.set_message(m_message.str().c_str()));
        m_log_record.set_message("");
        m_log_record.severity = log_severity::NONE;
    }

    void
    log::publish(const log_record& record)
    {
        s_log_sinks->publish(record);
    }

    bool
    log::enabled(log_severity s)
    {
        return true;
    }



}
