// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/shader_type.hpp"

#include <map>
#include <string_view>

namespace mge::dx11 {

    struct slang_compile_result
    {
        std::map<mge::shader_type, mge::buffer> shader_code;
    };

    slang_compile_result slang_compile(std::string_view source);

} // namespace mge::dx11
