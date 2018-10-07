// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/core/log_record.hpp"
#include "mge/core/configurable.hpp"

namespace mge {

    MGE_DECLARE_REF(log_sink);

    class MGE_CORE_EXPORT log_sink
            : public component<log_sink>,
              public configurable
    {
    protected:
        log_sink(bool synchronized=true);
        virtual void on_publish(const log_record& r) = 0;
    public:
        virtual ~log_sink() = default;
        void publish(const log_record& r);
    protected:
        bool       m_synchronized;
        std::mutex m_lock;
    };
}
