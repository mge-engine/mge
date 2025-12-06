// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/core/debugging.hpp"
#include "mge/core/trace_level.hpp"
#include "mge/core/trace_record.hpp"
#include "mge/core/trace_topic.hpp"

namespace mge::reflection {

    class core_reflector : public reflector
    {
    public:
        core_reflector() = default;
        ~core_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(function("breakpoint", &mge::breakpoint));
            mge(function("is_debugger_present", &mge::is_debugger_present));
            mge(function("breakpoint_if_debugging",
                         &mge::breakpoint_if_debugging));

            mge(type<mge::trace_level>().value("LEVEL_ERROR",
                                               mge::trace_level::LEVEL_ERROR));

            mge(type<mge::trace_record>()
                    .field("topic", &mge::trace_record::topic)
                    .field("message", &mge::trace_record::message)
                    .field("file", &mge::trace_record::file)
                    .field("function", &mge::trace_record::function)
                    .field("thread", &mge::trace_record::thread)
                    .field("sequence", &mge::trace_record::sequence)
                    .field("line", &mge::trace_record::line)
                    .field("level", &mge::trace_record::level));

            mge(type<mge::trace_topic>()
                    .constructor<std::string_view>()
                    .method("enabled", &mge::trace_topic::enabled)
                    .method("enable", &mge::trace_topic::enable)
                    .method("disable", &mge::trace_topic::disable)
                    .method(
                        "set_level",
                        static_cast<void (mge::trace_topic::*)(
                            mge::trace_level)>(&mge::trace_topic::set_level))
                    .method("set_level",
                            static_cast<void (mge::trace_topic::*)(uint8_t)>(
                                &mge::trace_topic::set_level))
                    .method("name", &mge::trace_topic::name)
                    .method("global", &mge::trace_topic::global));
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
