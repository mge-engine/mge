// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/shader_language.hpp"
#include "opengl.hpp"
#include <set>
#include <string>

namespace mge::opengl {

    class opengl_info
    {
    public:
        opengl_info();
        ~opengl_info();

        int                   major_version;
        int                   minor_version;
        mge::shader_language  shader_language;
        std::string           glsl_version_str;
        std::set<std::string> extensions;

    private:
        void install_debug_callback();
    };

} // namespace mge::opengl