// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/module.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/log.hpp"
#include <boost/algorithm/string.hpp>

MGE_DEFINE_LOG(APPLICATION)

namespace mge {
    application::application()
        :m_argc(0)
        ,m_return_code(0)
        ,m_update_thread(std::make_shared<update_thread>())
        ,m_update_rate(25)
        ,m_max_frame_skip(5)
        ,m_quit(false)
        ,m_setup_complete(false)
    {
        if (s_instance) {
            MGE_THROW(mge::illegal_state)
                      << "Cannot create duplicate application object";
        }
        s_instance = this;
    }

    application::~application()
    {
        if(s_instance != this) {
            mge::crash("Multiple application instances found!");
        }
        s_instance = nullptr;
    }

    application&
    application::instance()
    {
        if (s_instance) {
            return *s_instance;
        } else {
            MGE_THROW(mge::illegal_state) << "No application object has been created";
        }
    }

    void
    application::setup()
    {}

    void
    application::start()
    {
        m_update_thread->start();
        try {
            while(!m_setup_tasks.empty()) {
                auto t = m_setup_tasks.pop_front();
                m_update_thread->await(t);
            }
            m_setup_complete = true;
        } catch(...) {
            m_setup_tasks.clear();
            mge::rethrow();
        }
    }

    void
    application::add_application_thread(thread *t)
    {
        m_application_threads.add_thread(t);
    }

    void
    application::remove_application_thread(thread *t)
    {
        m_application_threads.remove_thread(t);
    }

    void
    application::join()
    {
        MGE_DEBUG_LOG(APPLICATION) << "Waiting for application quit";
        {
            std::unique_lock<decltype(m_quit_lock)> lock(m_quit_lock);
            while(!m_quit) {
                m_quit_condition.wait(lock);
            }
        }
        MGE_DEBUG_LOG(APPLICATION) << "Application shall quit, join all threads";
        m_application_threads.join_all();
        MGE_DEBUG_LOG(APPLICATION) << "Joining application update thread";
        if(m_update_thread && m_update_thread->joinable()) {
            m_update_thread->join();
        }
        MGE_DEBUG_LOG(APPLICATION) << "Application done";
    }

    void
    application::teardown()
    {}

    void
    application::set_arguments(int argc_, const char **argv_)
    {
        if(m_argc) {
            MGE_THROW(mge::illegal_state) << "Application arguments already set";
        }
        if(argc_ <= 0) {
            MGE_THROW(mge::illegal_argument) << "Argument count must be > 0";
        }
        m_argc = argc_;
        for(int i=0; i<argc_; ++i) {
            m_argv_values.emplace_back(argv_[i]);
        }
        m_commandline = boost::join(m_argv_values, " ");
        parse_config_arguments();
        for(const auto& s : m_argv_values) {
            m_argv.emplace_back(s.c_str());
        }
    }

    void
    application::parse_config_arguments()
    {
        auto it = m_argv_values.begin();
        // skip name
        ++it;
        while(it != m_argv_values.end()) {
            if(*it == "-c"
               || *it == "--config") {
                auto arg_it = it;
                ++it;
                if(it == m_argv_values.end()) {
                    MGE_THROW(mge::illegal_argument)
                              << "Configuration option expects an argument";
                }
                auto p = it->find_first_of("=");
                if(p == std::string::npos) {
                    MGE_THROW(mge::illegal_argument)
                              << "Configuration option in form key=value expected";
                }
                std::string key(it->begin(), it->begin()+p);
                std::string value(it->begin()+p+1, it->end());
                configuration::set_override(key, value);
                it = m_argv_values.erase(arg_it);
                it = m_argv_values.erase(it);
            } else {
                ++it;
            }
        }
    }

    void
    application::add_setup_task(const mge::task_ref& task)
    {
        if (m_setup_complete) {
            MGE_THROW(illegal_state) << "Setup tasks have already been processed";
        }
        if(!task) {
            MGE_THROW(illegal_argument) << "Invalid task reference";
        }
        m_setup_tasks.push_back(task);
    }

    void
    application::add_setup_task(std::function<void()>&& function)
    {
        auto tmp = std::make_shared<mge::task>(function);
        add_setup_task(tmp);
    }

    void
    application::await(const std::function<void()>& f)
    {
        if (m_update_thread->joinable()) {
            m_update_thread->await(f);
        } else {
            mge::async_executor::synchronous_executor().await(f);
        }
    }

    application *
    application::s_instance;

    void
    application::initialize()
    {
#ifdef MGE_OS_WINDOWS
        // prevent error popups and report all errors to process
        SetErrorMode(SEM_FAILCRITICALERRORS
                     | SEM_NOGPFAULTERRORBOX
                     | SEM_NOOPENFILEERRORBOX);
#endif
        module::load_all();
    }

    void
    application::update(uint64_t ts)
    {
        std::lock_guard<decltype(m_lock)> guard(m_lock);
        function_map<void(std::uint64_t)>::iteration_scope iteration_guard(m_update_listeners);
        for(const auto &el : m_update_listeners) {
            try {
                el.second(ts);
            } catch(const mge::exception& e) {
                MGE_ERROR_LOG(APPLICATION) << "Exception in update step: " << e.details();
            }
        }

    }

    void
    application::set_quit()
    {
        m_quit = true;

        std::lock_guard<decltype(m_lock)> guard(m_lock);
        function_map<void()>::iteration_scope iteration_guard(m_quit_listeners);
        for(const auto &el : m_quit_listeners) {
            try {
                el.second();
             } catch(const mge::exception& e) {
                MGE_ERROR_LOG(APPLICATION) << "Exception in quit listener: " << e;
             }
         }
         // post semaphore to wake up joining thread
         {
             std::unique_lock<decltype(m_quit_lock)> lock(m_quit_lock);
             m_quit_condition.notify_all();
         }
    }

    function_map<application::update_listener>::key_type
    application::add_update_listener(const application::update_listener& l)
    {
        if(l) {
            return m_update_listeners.insert(l);
        } else {
            MGE_THROW(mge::illegal_argument)
                      << "Try to add invalid update listener";
        }
    }

    bool
    application::remove_update_listener(function_map<application::update_listener>::key_type key)
    {
        return m_update_listeners.erase(key);
    }

    bool
    application::remove_quit_listener(function_map<application::quit_listener>::key_type key)
    {
        return m_quit_listeners.erase(key);
    }

    function_map<application::quit_listener>::key_type
    application::add_quit_listener(const application::quit_listener &listener)
    {
        if(listener) {
            return m_quit_listeners.insert(listener);
        } else {
            MGE_THROW(illegal_argument)
                      << "Try to add invalid quit listener";
        }
    }


    int
    application::run_application(int argc, const char **argv)
    {
        initialize();
        std::vector<std::string> implementations;
        application::implementations([&](const std::string& s){
            implementations.emplace_back(s);
        });
        if (implementations.empty()) {
            MGE_THROW(mge::no_such_element)
                      << "No application implementation found";
        }

        auto instance = application::create(implementations[0]);
        instance->set_arguments(argc, argv);
        instance->setup();
        instance->start();
        instance->join();
        instance->teardown();
        return instance->return_code();
    }

    MGE_REGISTER_COMPONENT(application);
}
