// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/trace_record.hpp"

#pragma once
namespace mge {

    MGE_DECLARE_REF(trace_sink);
    /**
     * @brief A trace sink.
     * A trace sink is the destination of trace records, to be displayed or
     * written into a file.
     */
    class MGECORE_EXPORT trace_sink
    {
    public:
        virtual ~trace_sink();

        /**
         * @brief Process a trace record.
         *
         * @param r trace record
         */
        virtual void publish(const trace_record &r) = 0;
    };
} // namespace mge