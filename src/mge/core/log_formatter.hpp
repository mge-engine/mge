// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/log_record.hpp"
#include "mge/core/component.hpp"

namespace mge {

    MGE_DECLARE_REF(log_formatter);

    /**
     * A log formatter formats a log record and writes it
     * to an output stream.
     */
    class MGE_CORE_EXPORT log_formatter
            : public component<log_formatter>
    {
    public:
        log_formatter() = default;
        virtual ~log_formatter() = default;

        /**
         * Format log record.
         * @param os output stream
         * @param r  log record
         */
        virtual void format(std::ostream& os,
                            const log_record& r) = 0;
    };
}
