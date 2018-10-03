// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/types.hpp"
#include "mge/core/component.hpp"
#include "mge/application/dllexport.hpp"

namespace mge {
    MGE_DECLARE_REF(application);

    class MGE_APPLICATION_EXPORT application
            : public component<application>
    {
    public:
        application();
        virtual ~application();
        static application& instance();
    private:
        static application *s_instance;
    };
}
