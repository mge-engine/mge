// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace_topic.hpp"

namespace mge {

    MGE_DEFINE_TRACE(MGE);
    MGE_DEFINE_TRACE(CORE);

    trace_topic::~trace_topic() {}

    bool trace_topic::enabled(trace_level l) const noexcept
    {
        uint8_t l_int = static_cast<uint8_t>(l);
        return (l_int & m_enabled_levels) != 0;
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

    const char *trace_topic::name() const noexcept { return m_topic; }

    bool trace_topic::global() const noexcept
    {
        return this == &__trace_topic_MGE;
    }

} // namespace mge