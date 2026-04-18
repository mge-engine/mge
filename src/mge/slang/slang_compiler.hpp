// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/shader_type.hpp"

#include <map>
#include <string>
#include <string_view>

namespace mge {

    enum class slang_target
    {
        DXBC,
        SPIRV,
        GLSL
    };

    struct slang_shader_code
    {
        mge::shader_type type;
        mge::buffer      binary_code;
        std::string      text_code;
        std::string      entry_point_name;
    };

    struct slang_compile_result
    {
        std::map<mge::shader_type, slang_shader_code> shader_code;
    };

    slang_compile_result slang_compile(slang_target     target,
                                       std::string_view source);

} // namespace mge
