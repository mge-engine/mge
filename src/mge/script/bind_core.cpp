// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/call_debugger.hpp"
#include "mge/core/crash.hpp"
#include "mge/core/semantic_version.hpp"
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
                function("call_debugger", &mge::call_debugger),
                type<trace_level>(),
                type<semantic_version>()
                    .constructor<uint32_t>()
                    .constructor<uint32_t, uint32_t>()
                    .constructor<uint32_t, uint32_t, uint32_t>()
                    .constructor<std::string_view>()
                    .method("empty", &semantic_version::empty)
                    .method("major", &semantic_version::major)
                    .method("minor", &semantic_version::minor)
                    .method("patch", &semantic_version::patch));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(core_script_binder,
                                mge::script_binder,
                                mgecore,
                                core);

} // namespace mge::script