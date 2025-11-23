// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/core/debugging.hpp"
#include "mge/core/trace_topic.hpp"

namespace mge::reflection {

    class core_reflector : public reflector
    {
    public:
        core_reflector() = default;
        ~core_reflector() = default;
        void reflect() const override
        {
            auto mge_module = module("mge");
            mge_module(function("breakpoint", &mge::breakpoint));
            mge_module(
                function("is_debugger_present", &mge::is_debugger_present));
            mge_module(function("breakpoint_if_debugging",
                                &mge::breakpoint_if_debugging));

            type<mge::trace_topic>().constructor<std::string_view>();
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 1> deps{{"std"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(core_reflector,
                                mge::reflection::reflector,
                                core);
} // namespace mge::reflection
