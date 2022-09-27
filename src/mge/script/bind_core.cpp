// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "mge/core/component.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/core/trace_level.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class component_registry_entry_base_proxy
        : public mge::script::proxy<component_registry_entry_base>
    {
    public:
        MGESCRIPT_PROXY_METHOD(std::string_view, name, (), (const noexcept));
    };

    class core_script_binder : public script_binder
    {
    public:
        core_script_binder() = default;

        void bind()
        {
            module("mge")(
                function("crash", &mge::crash),
                function("call_debugger", &mge::call_debugger),
                type<mge::trace_level>("trace_level")
                    .enum_value("NONE", mge::trace_level::NONE)
                    .enum_value("DEBUG", mge::trace_level::DEBUG)
                    .enum_value("INFO", mge::trace_level::INFO)
                    .enum_value("WARNING", mge::trace_level::WARNING)
                    .enum_value("ERROR", mge::trace_level::LEVEL_ERROR)
                    .enum_value("FATAL", mge::trace_level::FATAL)
                    .enum_value("ALL", mge::trace_level::ALL),
                type<mge::semantic_version>("semantic_version")
                    .constructor()
                    .constructor<uint32_t>()
                    .constructor<uint32_t, uint32_t>()
                    .constructor<uint32_t, uint32_t, uint32_t>()
                    .constructor<std::string_view>()
                    .copy_constructor()
                    .method("empty", &mge::semantic_version::empty)
                    .method("major", &mge::semantic_version::major)
                    .method("minor", &mge::semantic_version::minor)
                    .method("patch", &mge::semantic_version::patch)
                // module end
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(core_script_binder,
                                mge::script_binder,
                                mgecore,
                                core);
} // namespace mge::script