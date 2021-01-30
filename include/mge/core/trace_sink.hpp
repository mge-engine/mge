// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#include "mge/core/dllexport.hpp"
#include "mge/core/trace_record.hpp"

#pragma once
namespace mge {
    class MGECORE_EXPORT trace_sink
    {
    public:
        virtual ~trace_sink();
        virtual void publish(const trace_record &r) = 0;
    };
} // namespace mge