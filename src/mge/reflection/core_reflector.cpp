// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/core/clock.hpp"
#include "mge/core/component.hpp"
#include "mge/core/debugging.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/properties.hpp"
#include "mge/core/property_object.hpp"
#include "mge/core/semantic_version.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/string_pool.hpp"
#include "mge/core/thread.hpp"
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

            mge(type<mge::trace_level>()
                    .value("NONE", mge::trace_level::NONE)
                    .value("DEBUG", mge::trace_level::DEBUG)
                    .value("INFO", mge::trace_level::INFO)
                    .value("WARNING", mge::trace_level::WARNING)
                    .value("ERROR", mge::trace_level::ERROR)
                    .value("LEVEL_ERROR", mge::trace_level::LEVEL_ERROR)
                    .value("FATAL", mge::trace_level::FATAL)
                    .value("ALL", mge::trace_level::ALL));

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

            mge(type<clock>());

            mge(type<mge::component_base>("component")
                    .method("implementation_name",
                            &mge::component_base::implementation_name)
                    .method("alias_names", &mge::component_base::alias_names)
                    .static_method(
                        "create",
                        static_cast<std::shared_ptr<mge::component_base> (*)(
                            std::string_view,
                            std::string_view)>(&mge::component_base::create))
                    .static_method("component_registered",
                                   &mge::component_base::component_registered)
                    .static_method(
                        "implementation_registered",
                        &mge::component_base::implementation_registered)
                    .static_method(
                        "implementations",
                        static_cast<void (*)(
                            std::string_view,
                            const std::function<void(std::string_view)>&)>(
                            &mge::component_base::implementations)));

            mge(function("registered_components", &mge::registered_components));
            mge(function("registered_implementations",
                         &mge::registered_implementations));

            mge(type<mge::semantic_version>()
                    .constructor<>()
                    .constructor<uint32_t, uint32_t, uint32_t>()
                    .constructor<std::string_view>()
                    .method("major", &mge::semantic_version::major)
                    .method("minor", &mge::semantic_version::minor)
                    .method("patch", &mge::semantic_version::patch)
                    .method("empty", &mge::semantic_version::empty));

            mge(function("executable_name", &mge::executable_name));
            mge(function("executable_path", &mge::executable_path));

            mge(type<mge::stacktrace::frame>()
                    .method("name", &mge::stacktrace::frame::name)
                    .method("source_file", &mge::stacktrace::frame::source_file)
                    .method("source_line", &mge::stacktrace::frame::source_line)
                    .method("module", &mge::stacktrace::frame::module));

            mge(type<mge::stacktrace>().constructor<>().method(
                "size",
                &mge::stacktrace::size));

            mge(type<mge::string_pool>()
                    .constructor<>()
                    .method("get",
                            static_cast<std::string_view (mge::string_pool::*)(
                                std::string_view)>(&mge::string_pool::get))
                    .method("size", &mge::string_pool::size));

            mge(type<mge::properties>()
                    .constructor<>()
                    .method("exists",
                            static_cast<bool (mge::properties::*)(
                                const std::string_view&) const>(
                                &mge::properties::exists))
                    .method("erase",
                            static_cast<void (mge::properties::*)(
                                std::string_view)>(&mge::properties::erase))
                    .method("clear", &mge::properties::clear));

            mge(type<mge::property_object>()
                    .method("get_property",
                            static_cast<std::string (mge::property_object::*)(
                                const std::string&) const>(
                                &mge::property_object::get_property))
                    .method("get_property",
                            static_cast<std::string (mge::property_object::*)(
                                const std::string&,
                                const std::string&) const>(
                                &mge::property_object::get_property))
                    .method("exists_property",
                            &mge::property_object::exists_property)
                    .method("erase_property",
                            &mge::property_object::erase_property)
                    .method("clear_properties",
                            &mge::property_object::clear_properties));

            mge(type<mge::thread>()
                    .method("name", &mge::thread::name)
                    .method("joinable", &mge::thread::joinable)
                    .method("join", &mge::thread::join)
                    .method("detach", &mge::thread::detach)
                    .static_method("hardware_concurrency",
                                   &mge::thread::hardware_concurrency));

            mge(type<mge::mutex>()
                    .method("lock", &mge::mutex::lock)
                    .method("unlock", &mge::mutex::unlock)
                    .method("try_lock", &mge::mutex::try_lock));
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
