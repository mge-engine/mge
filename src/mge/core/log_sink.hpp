// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/core/configurable.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/log_record.hpp"

namespace mge {

    MGE_DECLARE_REF(log_sink);

    /**
     * @brief Log sink, i.e. log destination.
     */
    class MGE_CORE_EXPORT log_sink : public component<log_sink>,
                                     public configurable
    {
    protected:
        /**
         * @brief Construct a new log sink object.
         *
         * @param synchronized whether the publish function must be
         *                     synchronized.
         */
        log_sink(bool synchronized = true);

        /**
         * @brief Callback to publish a log record.
         *
         * @param r log record to publish
         */
        virtual void on_publish(const log_record &r) = 0;

    public:
        virtual ~log_sink() = default;
        /**
         * @brief Publishes a log record.
         *
         * @param r log record
         */
        void publish(const log_record &r);

    protected:
        bool       m_synchronized;
        std::mutex m_lock;
    };
} // namespace mge
