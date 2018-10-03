/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "mge/core/log_record.hpp"

#include <cstring>

namespace mge {
    log_record::log_record()
    :m_severity(log_severity::NONE)
    {
        m_category[0] = m_messageline[0] = '\0';
    }

    log_record::log_record(const log_record& r)
    {
        memcpy(this, &r, sizeof(log_record));
    }


    log_record&
    log_record::operator =(const log_record& r)
    {
        if(&r != this) {
            memcpy(this, &r, sizeof(log_record));
        }
        return *this;
    }

    void
    log_record::init(const char *category,
                     log_severity severity,
                     log_record::thread_id_type thread_id)
    {
        m_severity = severity;
        strncpy(m_category, category, sizeof(m_category));
        m_category[sizeof(m_category)-1] = '\0';
        m_messageline[0] = '\0';
        m_timestamp = std::chrono::system_clock::now();
        m_thread = thread_id;
    }
}
