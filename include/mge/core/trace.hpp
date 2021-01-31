// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/clock.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_topic.hpp"
#include <optional>
#include <sstream>

namespace mge {

    class MGECORE_EXPORT trace
    {
    public:
        trace(const trace_topic &topic, const trace_level level);
        ~trace();

        template <typename T> trace &operator<<(const T &value)
        {
            if (m_enabled) {
                (*m_stream) << value;
            }
            return *this;
        }

    private:
        void flush();

        const trace_topic &m_topic;
        bool               m_enabled;

        struct entry
        {
            clock::time_point time;
            uint32_t          sequence;
            trace_level       level;
        };

        std::optional<entry>             m_entry;
        std::optional<std::stringstream> m_stream;
    };

#define MGE_TRACE(TOPIC, LEVEL) trace(TOPIC, mge::trace_level::LEVEL)

} // namespace mge