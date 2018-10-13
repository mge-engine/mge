// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/script/dllexport.hpp"
#include "mge/core/component.hpp"

namespace mge {


    class script_engine : public component<script_engine>
    {
    protected:
        script_engine();
    public:
        virtual ~script_engine();
    };
}
