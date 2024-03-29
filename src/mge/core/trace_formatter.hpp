// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/trace_record.hpp"

#pragma once
namespace mge {

    MGE_DECLARE_REF(trace_formatter);

    /**
     * @brief Formats a trace record to an output stream.
     */
    class MGECORE_EXPORT trace_formatter
    {
    protected:
        trace_formatter() = default;

    public:
        trace_formatter(const trace_formatter&) = delete;
        trace_formatter& operator=(const trace_formatter&) = delete;

        virtual ~trace_formatter();
        /**
         * @brief Formats the trace record.
         *
         * @param stream target stream
         * @param r      trace record
         */
        virtual void format(std::ostream& stream, const trace_record& r) = 0;
    };

} // namespace mge