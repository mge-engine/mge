// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include "boost/boost_locale.hpp"
#include "mge/core/thread.hpp"
#include <atomic>
#include <codecvt>

namespace mge {

    static std::atomic<uint32_t> s_trace_sequence;

    trace::trace(trace_topic&                topic,
                 const trace_level           level,
                 const std::source_location& loc)
        : m_topic(topic)
        , m_enabled(topic.enabled(level))
    {
        if (m_enabled) {
            m_entry = entry();
            m_entry->time = clock::now();
            m_entry->level = level;
            m_entry->sequence = ++s_trace_sequence;
            m_entry->thread = mge::this_thread::system_id();
            m_entry->file = loc.file_name();
            m_entry->line = loc.line();
            m_entry->function = loc.function_name();
            m_stream = std::stringstream();
        }
    }

    trace::~trace()
    {
        if (m_enabled) {
            flush();
        }
    }

    void trace::flush()
    {
        trace_record r;

        std::string msg = m_stream->str();

        r.level = m_entry->level;
        r.sequence = m_entry->sequence;
        r.time = m_entry->time;
        r.thread = m_entry->thread;
        r.topic = &m_topic;
        r.file = m_entry->file;
        r.line = m_entry->line;
        r.function = m_entry->function;
        r.message = std::string_view(msg.begin(), msg.end());
        m_topic.publish(r);
    }

    trace& trace::operator<<(const std::wstring& value)
    {
        if (m_enabled) {
            (*m_stream) << boost::locale::conv::utf_to_utf<char>(value);
        }
        return *this;
    }

} // namespace mge