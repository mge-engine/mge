// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/bind.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include <mutex>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}
namespace mge::script {

    void bind_globals()
    {
        module()(type<void>(),
                 type<char>(),
                 type<unsigned char>(),
                 type<signed char>(),
                 type<short>(),
                 type<unsigned short>(),
                 type<int>(),
                 type<unsigned int>(),
                 type<int64_t>(),
                 type<uint64_t>(),
                 type<float>(),
                 type<double>());
    }

    void bind_std() { module("std")(type<std::string>()); }

    void bind_mge()
    {
        module("mge")(type<mge::trace_level>()
                          .enum_value("NONE", mge::trace_level::NONE)
                          .enum_value("DEBUG", mge::trace_level::DEBUG)
                          .enum_value("INFO", mge::trace_level::INFO)
                          .enum_value("WARNING", mge::trace_level::WARNING)
                          .enum_value("ERROR", mge::trace_level::LEVEL_ERROR)
                          .enum_value("FATAL", mge::trace_level::FATAL)
                          .enum_value("ALL", mge::trace_level::ALL));
    }

    std::once_flag s_bind_all_complete;

    void bind_all()
    {
        std::call_once(s_bind_all_complete, [] {
            try {
                mge::script::bind_globals();
                mge::script::bind_std();
                mge::script::bind_mge();
            } catch (const mge::exception& e) {
                MGE_ERROR_TRACE(SCRIPT) << "Binding global types failed: " << e;
                throw;
            }
        });
    }

} // namespace mge::script