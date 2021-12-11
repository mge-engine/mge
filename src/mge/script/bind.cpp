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

    static std::vector<mge::script::module> s_global_modules;

    void bind_globals()
    {
        s_global_modules.push_back(module()(type<void>(),
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
                                            type<double>()));
    }

    void bind_std()
    {
        s_global_modules.push_back(module("std")(type<std::string>()));
    }

    void bind_mge() { s_global_modules.push_back(module("mge")); }

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

    const std::vector<mge::script::module>& global_modules()
    {
        bind_all();
        return s_global_modules;
    }
} // namespace mge::script