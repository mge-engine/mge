// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif
#include <iostream>

namespace mge {

    MGE_DEFINE_TRACE(APPLICATION);

    MGE_REGISTER_COMPONENT(application);

    MGE_DEFINE_PARAMETER(std::string, application, name,
                         "Application name to instantiate");

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
        if (!configuration::loaded()) {
            configuration::load();
        }
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
        std::string_view used_application_name(application_name);
        std::string      application_name_parameter_value;
        try {
            if (!configuration::loaded()) {
                configuration::load();
            }

            if (used_application_name.empty()) {
                if (MGE_PARAMETER(application, name).has_value()) {
                    application_name_parameter_value =
                        MGE_PARAMETER(application, name).get();
                    used_application_name = application_name_parameter_value;
                }
            }

            MGE_DEBUG_TRACE(APPLICATION)
                << "Creating application '" << used_application_name << "'";
            auto app = application::create(used_application_name);
            MGE_DEBUG_TRACE(APPLICATION) << "Initializing application";
            app->initialize(argc, argv);
            MGE_DEBUG_TRACE(APPLICATION) << "Application setup";
            app->setup();
            MGE_DEBUG_TRACE(APPLICATION) << "Application run";
            app->run();
            MGE_DEBUG_TRACE(APPLICATION) << "Application teardown";
            app->teardown();
        } catch (const mge::exception &ex) {
            MGE_ERROR_TRACE(APPLICATION) << "Exception in application '"
                                         << used_application_name << "':";
            MGE_ERROR_TRACE(APPLICATION) << ex.details();
            return;
        } catch (const std::exception &ex) {
            MGE_ERROR_TRACE(APPLICATION) << "Exception in application '"
                                         << used_application_name << "':";
            MGE_ERROR_TRACE(APPLICATION) << ex.what();
            return;
        } catch (...) {
            MGE_ERROR_TRACE(APPLICATION) << "Unknown Exception in application '"
                                         << used_application_name << "'";
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
