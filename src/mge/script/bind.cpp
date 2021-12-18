// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/bind.hpp"
#include "mge/core/component.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/trace.hpp"

#include "mge/graphics/buffer_type.hpp"
#include "mge/graphics/data_type.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

#include <mutex>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
} // namespace mge

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
                          .enum_value("ALL", mge::trace_level::ALL),
                      type<mge::component_base>("component"),
                      function("executable_name", mge::executable_name));
    }

    void bind_mge_graphics()
    {
        module("mge")(
            type<mge::data_type>()
                .enum_value("UNKNOWN", mge::data_type::UNKNOWN)
                .enum_value("INT8", mge::data_type::INT8)
                .enum_value("UINT8", mge::data_type::UINT8)
                .enum_value("INT16", mge::data_type::INT16)
                .enum_value("UINT16", mge::data_type::UINT16)
                .enum_value("INT32", mge::data_type::INT32)
                .enum_value("UINT32", mge::data_type::UINT32)
                .enum_value("INT64", mge::data_type::INT64)
                .enum_value("UINT64", mge::data_type::UINT64)
                .enum_value("INT128", mge::data_type::INT128)
                .enum_value("UINT128", mge::data_type::UINT128)
                .enum_value("HALF", mge::data_type::HALF)
                .enum_value("FLOAT", mge::data_type::FLOAT)
                .enum_value("DOUBLE", mge::data_type::DOUBLE)
                .enum_value("LONG_DOUBLE", mge::data_type::LONG_DOUBLE),
            type<mge::buffer_type>()
                .enum_value("VERTEX", mge::buffer_type::VERTEX)
                .enum_value("INDEX", mge::buffer_type::INDEX)
                .enum_value("CONSTANT", mge::buffer_type::CONSTANT));
    }

    std::once_flag s_bind_all_complete;

    void bind_all()
    {
        std::call_once(s_bind_all_complete, [] {
            try {
                mge::script::bind_globals();
                mge::script::bind_std();
                mge::script::bind_mge();
                mge::script::bind_mge_graphics();
            } catch (const mge::exception& e) {
                MGE_ERROR_TRACE(SCRIPT) << "Binding global types failed: " << e;
                throw;
            }
        });
    }

} // namespace mge::script