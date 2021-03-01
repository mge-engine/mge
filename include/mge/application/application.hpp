// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroederprogress
// All rights reserved.
#pragma once
#include "mge/application/dllexport.hpp"
#include "mge/application/loop_target.hpp"
#include "mge/core/callback_map.hpp"
#include "mge/core/component.hpp"
#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace mge {

    class MGEAPPLICATION_EXPORT application : public component<application>,
                                              public loop_target
    {
    public:
        using update_listener_collection = callback_map<void(double)>;
        using update_listener     = update_listener_collection::function_type;
        using update_listener_key = update_listener_collection::key_type;

        using redraw_listener_collection = callback_map<void(double)>;
        using redraw_listener     = redraw_listener_collection::function_type;
        using redraw_listener_key = redraw_listener_collection::key_type;

        using input_listener_collection = callback_map<void()>;
        using input_listener     = input_listener_collection::function_type;
        using input_listener_key = input_listener_collection::key_type;

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
         * @brief Basic teardown of the application.
         * Default implementation does nothing.
         */
        virtual void teardown();

        /**
         * @brief Run the application.
         * This executes the input loop until the quit flag is set.
         */
        virtual void run();

        void input() override;

        void update(double delta) override;

        void present(double peek) override;

        /**
         * @brief Run an application implementation.
         * @param application application implementation name
         * @param argc argument count
         * @param argv argument values
         */
        static void main(std::string_view application, int argc,
                         const char **argv);

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

    private:
        std::vector<std::string>   m_arguments;
        volatile bool              m_quit;
        input_listener_collection  m_input_listeners;
        update_listener_collection m_update_listeners;
        redraw_listener_collection m_redraw_listeners;

    };
} // namespace mge
