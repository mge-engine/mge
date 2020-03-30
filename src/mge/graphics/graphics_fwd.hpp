// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/types.hpp"
namespace mge {
    class render_system;
    class monitor;
    class window;
    class render_context;
    class command_list;
    class index_buffer;
    class vertex_buffer;
    class shader_program;
    class pipeline;
    class texture_2d;
    class image;
    class drawable;

    MGE_DECLARE_REF(render_system);
    MGE_DECLARE_REF(monitor);
    MGE_DECLARE_REF(window);
    MGE_DECLARE_REF(render_context);
    MGE_DECLARE_REF(command_list);
    MGE_DECLARE_REF(index_buffer);
    MGE_DECLARE_REF(vertex_buffer);
    MGE_DECLARE_REF(shader_program);
    MGE_DECLARE_REF(pipeline);
    MGE_DECLARE_REF(texture_2d);
    MGE_DECLARE_REF(image);
    MGE_DECLARE_REF(drawable);
} // namespace mge
