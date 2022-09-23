// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/application/loop_context.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class application_script_binder : public script_binder
    {
    public:
        application_script_binder() = default;

        void bind()
        {
            module("mge")(
                type<mge::application>("application")(
                    /*type<application::input_listener>("input_listener"),
                    type<application::update_listener>("update_listener"),
                    type<application::redraw_listener>("redraw_listener"),
                    type<application::quit_listener>("quit_listener") */)
                    .method("argc", &application::argc)
                    .method("argv", &application::argv)
                    .method("set_quit", &application::set_quit)
                    .method("setup", &application::setup)
                    .method("async_setup", &application::async_setup)
                    .method("teardown", &application::teardown)
                    .method("run", &application::run)
                    .method("set_return_code", &application::set_return_code)
                    .method("return_code", &application::return_code)
                    /*
                    .method("add_input_listener",
                            &application::add_input_listener)
                    .method("remove_input_listener",
                            &application::remove_input_listener)
                    .method("add_update_listener",
                            &application::add_update_listener)
                    .method("remove_update_listener",
                            &application::remove_update_listener)
                    .method("add_redraw_listener",
                            &application::add_redraw_listener)
                    .method("remove_redraw_listener",
                            &application::remove_redraw_listener)
                    .method("add_quit_listener",
                            &application::add_quit_listener)
                    .method("remove_quit_listener",
                            &application::remove_quit_listener) */,
                type<loop_target>("loop_target")
                    .method("is_quit", &loop_target::is_quit)
                    .method("input", &loop_target::input)
                    .method("update", &loop_target::update)
                    .method("present", &loop_target::present),
                type<loop_context>("loop_context")
                    .constructor()
                    .field("cycle", &loop_context::cycle)
                    .field("delta", &loop_context::delta)
                    .field("peek", &loop_context::peek)
                // module end
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_script_binder,
                                mge::script_binder,
                                mgeapplication,
                                application);

} // namespace mge::script