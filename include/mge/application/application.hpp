// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/core/callback_map.hpp"
#include "mge/core/component.hpp"
#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace mge {

    class MGEAPPLICATION_EXPORT application : component<application>
    {
    public:
        using clock =
            std::conditional<std::chrono::high_resolution_clock::is_steady,
                             std::chrono::high_resolution_clock,
                             std::chrono::steady_clock>::type;

        using update_listener_collection = callback_map<void()>;
        using update_listener     = update_listener_collection::function_type;
        using update_listener_key = update_listener_collection::key_type;

        using redraw_listener = std::function<void()>;

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
        bool is_quit() const;

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
         * @brief Basic teardown of the application.
         * Default implementation does nothing.
         */
        virtual void teardown();

        /**
         * @brief Run the application.
         * This executes the input loop until the quit flag is set.
         */
        virtual void run();

        /**
         * @brief Run an application implementation.
         * @param application application implementation name
         * @param argc argument count
         * @param argv argument values
         */
        static void main(std::string_view application, int argc,
                         const char **argv);

        /**
         * @brief Add an update listener.
         *
         * @param l update listener
         * @return update_listener_key key for update listener registration
         */
        update_listener_key add_update_listener(const update_listener &l);

        /**
         * @brief Remove update listener.
         *
         * @param k listener key
         */
        void remove_update_listener(update_listener_key k);

        void set_redraw_listener(const redraw_listener &l);
        void clear_redraw_listener();

    private:
        std::vector<std::string>   m_arguments;
        volatile bool              m_quit;
        update_listener_collection m_update_listeners;
        redraw_listener            m_redraw_listener;
    };
} // namespace mge
