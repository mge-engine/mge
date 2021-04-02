// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/clock.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"
#include <string_view>

namespace mge {

    class trace_topic;
    /**
     * @brief A trace record.
     */
    struct MGECORE_EXPORT trace_record
    {
        const trace_topic *topic;    //!< trace topic
        std::string_view   message;  //!< trace message
        clock::time_point  time;     //!< time of the trace event
        uint32_t           thread;   //!< thread id
        uint32_t           sequence; //!< sequence number of trace event
        trace_level        level;    //!< trace event trace level
    };

} // namespace mge