// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/types.hpp"
#include "mge/core/component.hpp"
#include "mge/core/function_map.hpp"
#include "mge/core/async_executor.hpp"
#include "mge/core/thread_group.hpp"
#include "mge/application/dllexport.hpp"
#include "mge/application/application_fwd.hpp"
#include "mge/application/update_thread.hpp"

#include <vector>
#include <string>
#include <iostream>

#ifdef MGE_OS_WINDOWS
#  include <Windows.h>
#endif

namespace mge {



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
            , public async_executor
    {
    protected:
        application();
    public:
        /**
         * An application update listener. The update listener receives the
         * current nanoseconds since program start.
         *
         * It is called at most with the update frequency (25 times a
         * second by default).
         */
        using update_listener = std::function<void(std::uint64_t)>;

        /**
         * @brief Key type for update listener access.
         */
        using update_listener_key_type = function_map<update_listener>::key_type ;

        /**
         * A quit listener which shall be called when the
         * application shall quit.
         */
        using quit_listener = std::function<void()>;

        /**
         * @brief Key type for quit listener access.
         */
        using quit_listener_key_type = function_map<quit_listener>::key_type;

        /**
         * Destructor.
         */
        virtual ~application();

        /**
         * Access singleton instance.
         * @return singleton instance
         */
        static application& instance();

        /**
         * Callback method called before start is called.
         */
        virtual void setup();

        /**
         * Starts the application.
         */
        virtual void start();

        /**
         * Joins the application, waits until the last application
         * thread has been finished.
         */
        virtual void join();

        /**
         * Callback method called after teardown is called.
         */
        virtual void teardown();

        /**
         * Execute asynchronously in update thread.
         * @param f function to execute
         */
        void await(const std::function<void()>& f);

        /**
         * Add setup task which will be executed in update thread
         * after @c start method is done.
         * @param task task to execute
         */
        void add_setup_task(const mge::task_ref& task);

        /**
         * Add setup task which will be executed in update thread
         * after @c start method is done.
         * @param function function to execute
         */
        void add_setup_task(std::function<void()>&& function);

        /**
         * Add an application thread that will be joined in
         * graceful shutdown.
         * @param t thread
         */
        void add_application_thread(thread *t);

        /**
         * Remove an application thread that will be joined in
         * graceful shutdown.
         * @param t thread
         */
        void remove_application_thread(thread *t);

        /**
         * Runs the only found implementation of the application
         * interface.
         * @param argc argument count
         * @param argv argument values
         * @return application return code
         */
        static int run_application(int argc, const char **argv);

#ifdef MGE_OS_WINDOWS
        /**
         * Runs the only found implementation of the application
         * interface in a windowed instance.
         * @param instance      instance handle
         * @param cmdline       command line string
         * @param show_command  whether application shall start visible
         * @return return code for @c WinMain function
         */
        static int win_main(HINSTANCE instance,
                            const char *cmdline,
                            int show_command);
#endif
        /**
         * Get argument count.
         * @return argument count
         */
        int argc() const noexcept
        {
            return m_argc;
        }

        /**
         * Get argument values.
         * @return argument value vector
         */
        const char **argv() const noexcept
        {
            return (const char **)m_argv.data();
        }

        /**
         * Get the command line.
         * @return command line
         */
        const std::string& commandline() const noexcept
        {
            return m_commandline;
        }

        /**
         * Program name.
         * @return program name
         */
        const std::string& program_name() const noexcept
        {
            return m_argv_values[0];
        }

        /**
         * Get the return code that should be passed to the OS.
         * @return return code
         */
        int return_code() const noexcept
        {
            return m_return_code;
        }

        /**
         * Sets the return code.
         * @param returncode return code to set
         */
        void set_return_code(int returncode) noexcept
        {
            m_return_code = returncode;
        }

        /**
         * Get update rate (Hz).
         * @return update rate
         */
        uint32_t update_rate() const noexcept
        {
            return m_update_rate;
        }

        /**
         * Get maximum number of skipped frames.
         * @return max number of skipped frames
         */
        uint32_t max_frame_skip() const noexcept
        {
            return m_max_frame_skip;
        }

        /**
         * Return state of quit flag.
         * @return quit flag
         */
        bool is_quit() const noexcept
        {
            return m_quit;
        }

        /**
         * Set the quit flag.
         */
        void set_quit();

        /**
         * Initialize use of application.
         * Must be called before using the framework.
         */
        static void initialize();

        /**
         * Update the application. This method calls all registered update
         * listeners, and passes the given time stamp as current time.
         * Exceptions raised by update listeners are logged, but processing
         * continues.
         * @param ts time stamp of current time, see @c clock::now()
         */
        virtual void update(uint64_t ts);

        /**
         * Add an update listener.
         * @param l listener
         * @return listener key that can be used to remove the listener
         */
        function_map<update_listener>::key_type
        add_update_listener(const update_listener& l);

        /**
         * Remove update listener by key.
         * @param key listener key
         * @return @c true if entry found
         */
        bool remove_update_listener(function_map<update_listener>::key_type key);

        /**
         * Add a quit listener - get notified if the application shall quit.
         * @param listener listener to add
         * @return listener key
         */
        function_map<quit_listener>::key_type
        add_quit_listener(const quit_listener& listener);

        /**
         * Remove quit listener by listener key.
         * @param key listener key
         * @return @c true if entry found
         */
        bool remove_quit_listener(function_map<quit_listener>::key_type key);

    private:
        void set_arguments(int argc, const char **argv);
        void parse_config_arguments();

        static application *s_instance;

        int m_argc;
        int m_return_code;
        std::vector<const char *> m_argv;
        std::vector<std::string> m_argv_values;
        std::string m_commandline;
        update_thread_ref m_update_thread;
        thread_group m_application_threads;
        std::condition_variable m_quit_condition;
        std::mutex m_quit_lock;
        std::recursive_mutex m_lock;
        function_map<void(std::uint64_t)> m_update_listeners;
        function_map<void()> m_quit_listeners;
        task_queue m_setup_tasks;
        uint32_t m_update_rate;
        uint32_t m_max_frame_skip;
        volatile bool m_quit;
        bool m_has_std_streams;
        bool m_setup_complete;
    };
}

#define MGE_MAINFUNCTION int main(int argc, const char **argv)                \
{                                                                             \
    try {                                                                     \
        return ::mge::application::run_application(argc, argv);               \
    } catch(const mge::exception& ex) {                                       \
        try {                                                                 \
            std::cerr << "Exception in main program: " << ex << std::endl;    \
        } catch(...) {                                                        \
        }                                                                     \
        return 1;                                                             \
    } catch(const std::exception& ex) {                                       \
        std::cerr << "Exception in main program: " << ex.what() << std::endl; \
        return 1;                                                             \
    }                                                                         \
}

#define MOGE_WINMAINFUNCTION int CALLBACK WinMain(HINSTANCE hInstance,       \
    HINSTANCE hPrevInstance,                                                 \
    LPSTR lpCmdLine,                                                         \
    int nCmdShow)                                                            \
{                                                                            \
    try {                                                                    \
        return ::mge::application::win_main(hInstance, lpCmdLine, nCmdShow); \
    } catch (const std::exception&) {                                        \
        return 1;                                                            \
    }                                                                        \
}

