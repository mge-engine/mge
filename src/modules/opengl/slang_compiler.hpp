// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/shader_type.hpp"

#include <map>
#include <string>
#include <string_view>

namespace mge::opengl {

    struct slang_compile_result
    {
        std::map<mge::shader_type, std::string> shader_code;
    };

    slang_compile_result slang_compile(std::string_view source);

} // namespace mge::opengl
