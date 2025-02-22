// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/application/loop.hpp"
#include "mge/application/loop_target.hpp"
#include "mge/script/function.hpp"
#include "mge/script/implement_method.hpp"
#include "mge/script/module.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"
namespace mge::script {

    class proxy_loop_target : public mge::script::proxy<mge::loop_target>
    {
    public:
        proxy_loop_target() = default;
        virtual ~proxy_loop_target() = default;

        MGE_IMPLEMENT_METHOD(bool, is_quit, (), const);
        MGE_IMPLEMENT_METHOD(void, input, (uint64_t));
        MGE_IMPLEMENT_METHOD(void, update, (uint64_t, double, double));
        MGE_IMPLEMENT_METHOD(void, present, (uint64_t, double));
    };

    class application_script_binder : public script_binder
    {
    public:
        application_script_binder() = default;

        void bind() override
        {

            mge::script::module mge("mge");
            mge(type<mge::loop_target>()
                    .method("is_quit", &mge::loop_target::is_quit)
                    .method("input", &mge::loop_target::input)
                    .method("update", &mge::loop_target::update)
                    .method("present", &mge::loop_target::present),
                type<mge::application>()
                    .base<mge::loop_target>()
                    .base<mge::component_base>()
                    .method("argc", &mge::application::argc)
                    .method("argv", &mge::application::argv)
                    .method("is_quit", &mge::application::is_quit)
                    .method("set_quit", &mge::application::set_quit)
                    .method("initialize", &mge::application::initialize)
                    .method("setup", &mge::application::setup)
                    .method("async_setup", &mge::application::async_setup)
                    .method("teardown", &mge::application::teardown)
                    .method("run", &mge::application::run)
                    .method("set_return_code",
                            &mge::application::set_return_code)
                    .method("return_code", &mge::application::return_code)
                    .function("main",
                              static_cast<int (*)(int, const char**)>(
                                  mge::application::main))
                    .function("main",
                              static_cast<
                                  int (*)(std::string_view, int, const char**)>(
                                  mge::application::main))
                    .method("add_input_listener",
                            &mge::application::add_input_listener)
                    .method("remove_input_listener",
                            &mge::application::remove_input_listener)
                    .method("add_update_listener",
                            &mge::application::add_update_listener)
                    .method("remove_update_listener",
                            &mge::application::remove_update_listener)
                    .method("add_redraw_listener",
                            &mge::application::add_redraw_listener)
                    .method("remove_redraw_listener",
                            &mge::application::remove_redraw_listener)
                    .method("add_quit_listener",
                            &mge::application::add_quit_listener)
                    .method("remove_quit_listener",
                            &mge::application::remove_quit_listener)
                    .function("instance", &mge::application::instance),
                type<mge::loop>().method("run", &mge::loop::run)
                //
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_script_binder,
                                mge::script_binder,
                                mgeapplication,
                                application);

} // namespace mge::script