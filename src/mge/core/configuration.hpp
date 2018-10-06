// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
namespace mge {

    class MGE_CORE_EXPORT configuration
    {
    public:
        configuration();
        configuration(const configuration& c);
        configuration(configuration&& c);
        ~configuration();

        configuration& operator =(const configuration& c);
        configuration& operator =(configuration&& c);
    };
}
