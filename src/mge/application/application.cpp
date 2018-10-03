// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    application::application()
    {
        if (s_instance) {
            MGE_THROW(mge::illegal_state(),
                      "Cannot create duplicate application object");
        }
        s_instance = this;
    }

    application::~application()
    {
        if(s_instance != this) {
            // TODO: Crash due to inconsistency
        }
        s_instance = nullptr;
    }

    application&
    application::instance()
    {
        if (s_instance) {
            return *s_instance;
        } else {
            MGE_THROW(mge::illegal_state(),
                      "No application object has been created");
        }
    }

    application *
    application::s_instance;

    MGE_REGISTER_COMPONENT(application);
}
