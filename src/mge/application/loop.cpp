// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/application/loop.hpp"
#include "mge/application/loop_target.hpp"
namespace mge {

    loop_target::~loop_target() {}

    MGE_REGISTER_COMPONENT(loop);
} // namespace mge
