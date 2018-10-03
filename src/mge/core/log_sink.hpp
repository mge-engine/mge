// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/component.hpp"
#include "mge/core/log_record.hpp"

namespace mge {

    class MGE_CORE_EXPORT log_sink
            : public component<log_sink>
    {
    public:
        log_sink() = default;
        virtual ~log_sink () = default;
        virtual void publish(log_record& r) = 0;
    };
}
