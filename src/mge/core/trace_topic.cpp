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

    const char *trace_topic::name() const noexcept { return m_topic; }

    bool trace_topic::global() const noexcept
    {
        return this == &__trace_topic_MGE;
    }

} // namespace mge