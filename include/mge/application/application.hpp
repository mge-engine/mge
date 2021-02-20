// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/core/component.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace mge {

    class MGEAPPLICATION_EXPORT application : component<application>
    {
    public:
        application();
        ~application();

        int              argc() const;
        std::string_view argv(int index) const;
        bool             is_quit() const;
        void             set_quit();

        void initialize(int argc, const char **argv);

        virtual void setup();

        virtual void update();

        virtual void teardown();

        void run();

        /**
         * @brief Run an application implementation.
         * @param application application implementation name
         * @param argc argument count
         * @param argv argument values
         */
        static void main(std::string_view application, int argc, const char **argv);
    private:
        std::vector<std::string> m_arguments;
        volatile bool            m_quit;
    };
} // namespace mge
