// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/atexit.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/debugging.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    class core_script_binder : public script_binder
    {
    public:
        core_script_binder() = default;

        void bind() override
        {
            mge::script::module mge("mge");
            mge(function("crash", &mge::crash),
                function("breakpoint", &mge::breakpoint),
                function("breakpoint_if_debugging",
                         &mge::breakpoint_if_debugging),
                function("is_debugger_present", &mge::is_debugger_present),
                type<trace_level>(),
                type<semantic_version>()
                    .constructor<uint32_t>()
                    .constructor<uint32_t, uint32_t>()
                    .constructor<uint32_t, uint32_t, uint32_t>()
                    .constructor<std::string_view>()
                    .method("empty", &semantic_version::empty)
                    .method("major", &semantic_version::major)
                    .method("minor", &semantic_version::minor)
                    .method("patch", &semantic_version::patch),
                type<atexit>()
                    .function("stop_processing", &atexit::stop_processing)
                    .function("run", &atexit::run),
                // type<buffer>(),
                type<configuration>()
                    .function("register_parameter",
                              &configuration::register_parameter)
                    .function("unregister_parameter",
                              &configuration::unregister_parameter)
                    // .function(
                    //     "find_parameter",
                    //     static_cast<basic_parameter& (*)(std::string_view,
                    //                                      std::string_view)>(
                    //         &configuration::find_parameter))
                    .function("load", &configuration::load)
                    .function("evaluate_command_line",
                              &configuration::evaluate_command_line)
                    .function("store", &configuration::store)
                    .function("loaded", &configuration::loaded)
                    .function("root", &configuration::root),

                type<mge::exception>().base<std::exception>(),
                type<mge::illegal_state>().base<mge::exception>(),
                type<mge::illegal_argument>().base<mge::exception>(),
                type<mge::out_of_range>().base<mge::exception>(),
                type<mge::duplicate_element>().base<mge::exception>(),
                type<mge::bad_cast>().base<mge::exception>(),
                type<no_such_element>().base<mge::exception>(),
                type<runtime_exception>().base<mge::exception>(),
                type<not_yet_implemented>().base<mge::exception>(),
                type<null_pointer>().base<mge::exception>(),
                type<out_of_memory>().base<mge::exception>(),
                type<numeric_overflow>().base<mge::exception>(),
                type<not_implemented>().base<mge::exception>(),
                type<pure_virtual_method>().base<mge::exception>()

                //
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(core_script_binder,
                                mge::script_binder,
                                mgecore,
                                core);

} // namespace mge::script