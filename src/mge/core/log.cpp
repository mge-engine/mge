// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/log.hpp"
#include "mge/core/log_sink.hpp"
#include "mge/core/singleton.hpp"

namespace mge {

    class log_sink_collection
    {
    public:
        log_sink_collection()
        {}

        typedef std::map<std::string, log_sink_ref> sink_map_t;

        void publish(const log_record& r)
        {
            if(m_sinks.empty()) {
                init_sinks();
            }

            for(const auto& p: m_sinks) {
                p.second->publish(r);
            }
        }
    private:
        void init_sinks()
        {
            m_sinks["stdout"] = log_sink::create("stdout");
        }
        sink_map_t m_sinks;
    };

    static singleton<log_sink_collection> s_log_sinks;


    log::log(const char *topic)
    {
        m_log_record.category  = topic;
        m_log_record.severity  = log_severity::NONE;
        m_log_record.thread_id = mge::this_thread::get_id();
        m_log_record.message = "";
    }

    log::~log()
    {}

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
