// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"
#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif
#include <iostream>

namespace mge {
    MGE_REGISTER_COMPONENT(application);

    application::application() : m_quit(false) {}

    void application::initialize(int argc, const char **argv)
    {
        while (argc) {
            m_arguments.emplace_back(*argv);
            --argc;
            ++argv;
        }

#ifdef MGE_OS_WINDOWS
        // prevent error popups and report all errors to process
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX |
                     SEM_NOOPENFILEERRORBOX);
#endif
    }

    application::~application() {}

    int application::argc() const
    {
        return static_cast<int>(m_arguments.size());
    }

    std::string_view application::argv(int index) const
    {
        if (index < 0 || static_cast<size_t>(index) > m_arguments.size()) {
            MGE_THROW(out_of_range) << "Invalid index " << index;
        }
        return m_arguments[static_cast<size_t>(index)];
    }

    void application::setup() {}

    void application::teardown() {}

    void application::run() {}

    void application::main(std::string_view application_name, int argc,
                           const char **argv)
    {
        try {
            auto app = application::create(application_name);
            app->initialize(argc, argv);
            app->setup();
            app->run();
            app->teardown();
        } catch (const mge::exception &ex) {
            std::cerr << "Exception in application " << application_name
                      << std::endl
                      << "    " << ex.details() << std::endl;
            return;
        } catch (const std::exception &ex) {
            std::cerr << "Exception in application " << application_name
                      << std::endl
                      << "    " << ex.what() << std::endl;
            return;
        } catch (...) {
            std::cerr << "Unknown exception in application " << application_name
                      << std::endl;
            return;
        }
    }

    application::input_listener_key
    application::add_input_listener(const application::input_listener &l)
    {
        return m_input_listeners.insert(l);
    }

    void application::remove_input_listener(application::input_listener_key k)
    {
        m_input_listeners.erase(k);
    }

    application::update_listener_key
    application::add_update_listener(const application::update_listener &l)
    {
        return m_update_listeners.insert(l);
    }

    void application::remove_update_listener(application::update_listener_key k)
    {
        m_update_listeners.erase(k);
    }

    application::redraw_listener_key
    application::add_redraw_listener(const application::redraw_listener &l)
    {
        return m_redraw_listeners.insert(l);
    }

    void application::remove_redraw_listener(application::redraw_listener_key k)
    {
        m_redraw_listeners.erase(k);
    }

    bool application::is_quit() const { return m_quit; }

    void application::set_quit() { m_quit = true; }

    void application::input()
    {
        for (const auto &l : m_input_listeners) {
            l();
        }
    }

    void application::update(double delta)
    {
        for (const auto &l : m_update_listeners) {
            l(delta);
        }
    }

    void application::present(double peek)
    {
        for (const auto &l : m_redraw_listeners) {
            l(peek);
        }
    }
} // namespace mge
