// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_topic.hpp"
#include <optional>
namespace mge {

    class MGECORE_EXPORT trace
    {
    public:
        trace(const trace_topic &topic, const trace_level level);
        ~trace();

        template <typename T> trace &operator<<(const T &value)
        {
            return *this;
        }

    private:
    };

#define MGE_TRACE(TOPIC, LEVEL) trace(TOPIC, mge::trace_level::LEVEL)

} // namespace mge