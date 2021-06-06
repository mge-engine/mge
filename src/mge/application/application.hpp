// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/application/loop_target.hpp"
#include "mge/core/callback_map.hpp"
#include "mge/core/component.hpp"
#include "mge/core/thread.hpp"
#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;
namespace mge {

    /**
     * @brief Application framework.
     *
     */
    class MGEAPPLICATION_EXPORT application : public component<application>,
                                              public loop_target
    {
    public:
        using update_listener_collection = callback_map<double>;
        using update_listener     = update_listener_collection::function_type;
        using update_listener_key = update_listener_collection::key_type;

        using redraw_listener_collection = callback_map<uint64_t, double>;
        using redraw_listener     = redraw_listener_collection::function_type;
        using redraw_listener_key = redraw_listener_collection::key_type;

        using input_listener_collection = callback_map<void>;
        using input_listener     = input_listener_collection::function_type;
        using input_listener_key = input_listener_collection::key_type;

        using quit_listener_collection = callback_map<void>;
        using quit_listener     = quit_listener_collection::function_type;
        using quit_listener_key = quit_listener_collection::key_type;

        application();
        ~application();

        /**
         * @brief Argument count.
         *
         * @return number of arguments
         */
        int argc() const;

        /**
         * @brief Argument value.
         *
         * @param index index of argument
         * @return argument value
         */
        std::string_view argv(int index) const;

        /**
         * @brief Shall the application quit?
         *
         * @return @c true if application shall quit
         */
        bool is_quit() const override;

        /**
         * @brief Signal that application shall quit.
         */
        void set_quit();

        void initialize(int argc, const char **argv);

        /**
         * @brief Basic setup of the application.
         * Default implementation does nothing.
         */
        virtual void setup();

        /**
         * @brief Setup that is started asynchronously to the event loop.
         */
        virtual void async_setup();

        /**
         * @brief Basic teardown of the application.
         * Default implementation does nothing.
         */
        virtual void teardown();

        /**
         * @brief Run the application.
         * This executes the input loop until the quit flag is set.
         */
        virtual void run();

        void input(uint64_t cycle) override;

        void update(uint64_t cycle, double delta) override;

        void present(uint64_t cycle, double peek) override;

        void set_return_code(int return_code);

        int return_code() const noexcept;

        /**
         * @brief Run an application implementation.
         * @param application application implementation name
         * @param argc argument count
         * @param argv argument values
         * @return return code passed to operating system
         */
        static int main(std::string_view application, int argc,
                        const char **argv);

        /**
         * @brief Run default application implementation.
         *
         * @param argc argument count
         * @param argv argument values
         * @return return code passed to operating system
         */
        static inline int main(int argc, const char **argv)
        {
            return main(""sv, argc, argv);
        }

        /**
         * @brief Add an input listener.
         *
         * @param l update listener
         * @return key for update listener registration
         */
        input_listener_key add_input_listener(const input_listener &l);

        /**
         * @brief Remove update listener.
         *
         * @param k listener key
         */
        void remove_input_listener(input_listener_key k);

        /**
         * @brief Add an update listener.
         *
         * @param l update listener
         * @return key for update listener registration
         */
        update_listener_key add_update_listener(const update_listener &l);

        /**
         * @brief Remove update listener.
         *
         * @param k listener key
         */
        void remove_update_listener(update_listener_key k);

        /**
         * @brief Add a redraw listener.
         *
         * @param l listener
         * @return key for redraw listener registration
         */
        redraw_listener_key add_redraw_listener(const redraw_listener &l);

        /**
         * @brief Remove redraw listener.
         * @param k listener key
         */
        void remove_redraw_listener(redraw_listener_key k);

        /**
         * @brief Add a quit listener.
         *
         * @param l update listener
         * @return key for update listener registration
         */
        quit_listener_key add_quit_listener(const quit_listener &l);

        /**
         * @brief Remove update listener.
         *
         * @param k listener key
         */
        void remove_quit_listener(quit_listener_key k);

        /**
         * @brief Access singleton/current instance.
         *
         * @return current application instance
         */
        static application *instance();

    protected:
        const std::vector<std::string> &arguments() const;

    private:
        std::vector<std::string> m_arguments;
        int                      m_return_code;
        volatile bool            m_quit;
        mge::thread::id          m_main_thread_id;

        static application *s_instance;

        input_listener_collection  m_input_listeners;
        update_listener_collection m_update_listeners;
        redraw_listener_collection m_redraw_listeners;
        quit_listener_collection   m_quit_listeners;
    };
} // namespace mge

#define MGE_MAINFUNCTION                                                       \
    int main(int argc, const char **argv)                                      \
    {                                                                          \
        return mge::application::main(argc, argv);                             \
    }
