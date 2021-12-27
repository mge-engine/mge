// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/module.hpp"
#include <vector>

namespace mge::script {

    MGESCRIPT_EXPORT void bind_all();
    MGESCRIPT_EXPORT void bind_globals();
    MGESCRIPT_EXPORT void bind_std();
    MGESCRIPT_EXPORT void bind_mge();

}