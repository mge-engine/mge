// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/application/loop.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/module.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif
#include <iostream>

namespace mge {

    MGE_DEFINE_TRACE(APPLICATION);

    MGE_REGISTER_COMPONENT(application);

    MGE_DEFINE_PARAMETER(std::string,
                         application,
                         name,
                         "Application name to instantiate");
    MGE_DEFINE_PARAMETER(std::string,
                         application,
                         loop,
                         "Application loop implementation to instantiate");

    application* application::s_instance;

    application::application()
        : m_return_code(0)
        , m_quit(false)
    {

        if (s_instance) {
            MGE_THROW(illegal_state)
                << "Can only have one application instance";
        }

        m_main_thread_id = mge::this_thread::get_id();

        s_instance = this;
    }

    void application::initialize(int argc, const char** argv)
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

    application::~application()
    {
        s_instance = nullptr;
    }

    application* application::instance()
    {
        if (!s_instance) {
            MGE_THROW(mge::illegal_state)
                << "Application instance requested, but none available";
        }

        return s_instance;
    }

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

    void application::async_setup() {}

    void application::teardown() {}

    void application::run()
    {
        std::string loop_implementation("simple");
        if (MGE_PARAMETER(application, loop).has_value()) {
            loop_implementation = MGE_PARAMETER(application, loop).get();
        }
        MGE_DEBUG_TRACE_STREAM(APPLICATION)
            << "Using loop implementation: " << loop_implementation;
        auto loop_instance = mge::loop::create(loop_implementation);
        loop_instance->run(*this);
        MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Main loop finished";
    }

    int application::main(std::string_view application_name,
                          int              argc,
                          const char**     argv)
    {
        std::string_view used_application_name(application_name);
        std::string      application_name_parameter_value;
        try {
            if (!configuration::loaded()) {
                configuration::load();
            }
            std::vector<const char*> new_argv;
            for (int i = 0; i < argc; ++i) {
                new_argv.push_back(argv[i]);
            }
            module::load_all();
            configuration::evaluate_command_line(new_argv);

            if (used_application_name.empty()) {
                if (MGE_PARAMETER(application, name).has_value()) {
                    MGE_DEBUG_TRACE_STREAM(APPLICATION)
                        << "Application name parameter found";
                    application_name_parameter_value =
                        MGE_PARAMETER(application, name).get();
                    used_application_name = application_name_parameter_value;
                }
                if (used_application_name.empty()) {
                    MGE_DEBUG_TRACE_STREAM(APPLICATION)
                        << "No application name parameter found, falling back "
                           "to executable name";
                    application_name_parameter_value = mge::executable_name();
                    used_application_name = application_name_parameter_value;
                }
            }

            MGE_DEBUG_TRACE_STREAM(APPLICATION)
                << "Create application '" << used_application_name << "'";
            auto app = application::create(used_application_name);
            if (!app) {
                return 1;
            }
            MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Application initialize";
            app->initialize(argc, argv);
            MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Application setup";
            app->setup();
            MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Application run";
            app->run();
            MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Remove all listeners";
            app->clear_listeners();
            MGE_DEBUG_TRACE_STREAM(APPLICATION) << "Application teardown";
            app->teardown();
            auto rc = app->return_code();
            MGE_DEBUG_TRACE_STREAM(APPLICATION)
                << "Application '" << used_application_name
                << "' has finished with return code " << rc;
            // store config if all is well
            if (!rc) {
                configuration::store();
            }
            return rc;
        } catch (const mge::exception& ex) {
            MGE_ERROR_TRACE_STREAM(APPLICATION) << "Exception in application '"
                                         << used_application_name << "':";
            MGE_ERROR_TRACE_STREAM(APPLICATION) << ex.details();
            return 1;
        } catch (const std::exception& ex) {
            MGE_ERROR_TRACE_STREAM(APPLICATION) << "Exception in application '"
                                         << used_application_name << "':";
            MGE_ERROR_TRACE_STREAM(APPLICATION) << ex.what();
            return 1;
        } catch (...) {
            MGE_ERROR_TRACE_STREAM(APPLICATION) << "Unknown exception in application '"
                                         << used_application_name << "'";
            return 1;
        }
    }

    application::input_listener_key
    application::add_input_listener(const application::input_listener& l)
    {
        return m_input_listeners.insert(l);
    }

    void application::remove_input_listener(application::input_listener_key k)
    {
        m_input_listeners.erase(k);
    }

    application::update_listener_key
    application::add_update_listener(const application::update_listener& l)
    {
        return m_update_listeners.insert(l);
    }

    void application::remove_update_listener(application::update_listener_key k)
    {
        m_update_listeners.erase(k);
    }

    application::redraw_listener_key
    application::add_redraw_listener(const application::redraw_listener& l)
    {
        return m_redraw_listeners.insert(l);
    }

    void application::remove_redraw_listener(application::redraw_listener_key k)
    {
        m_redraw_listeners.erase(k);
    }

    application::quit_listener_key
    application::add_quit_listener(const application::quit_listener& l)
    {
        return m_quit_listeners.insert(l);
    }

    void application::remove_quit_listener(application::quit_listener_key k)
    {
        m_quit_listeners.erase(k);
    }

    bool application::is_quit() const
    {
        return m_quit;
    }

    void application::set_quit()
    {
        m_quit = true;
    }

    void application::input(uint64_t cycle)
    {
        m_input_listeners();
    }

    void application::update(uint64_t cycle, double elapsed, double delta)
    {
        m_update_listeners(cycle, elapsed, delta);
    }

    void application::present(uint64_t cycle, double peek)
    {
        m_redraw_listeners(cycle, peek);
    }

    void application::set_return_code(int return_code)
    {
        m_return_code = return_code;
    }

    int application::return_code() const noexcept
    {
        return m_return_code;
    }

    const std::vector<std::string>& application::arguments() const
    {
        return m_arguments;
    }

    void application::clear_listeners()
    {
        m_input_listeners.clear();
        m_update_listeners.clear();
        m_redraw_listeners.clear();
        m_quit_listeners.clear();
    }

} // namespace mge
