// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/types.hpp"
#include "mge/core/component.hpp"
#include "mge/application/dllexport.hpp"

namespace mge {
    MGE_DECLARE_REF(application);


    /**
     * Application base class.
     *
     * This base class implements main loop which performs
     * updates to the application state at a constant rate (update rate)
     * allowing to skip at most a certain number of frames in display.
     *
     */
    class MGE_APPLICATION_EXPORT application
            : public component<application>
    {
    public:
        application();
        virtual ~application();
        static application& instance();

        /**
         * Starts the application.
         */
        virtual void start();

        /**
         * Joins the application, waits until the last application
         * thread has been finished.
         */
        virtual void join();
    private:
        static application *s_instance;
    };
}
