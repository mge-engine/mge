// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_context.hpp"
#include "mge/core/singleton.hpp"
#include "mge/script/script_binder.hpp"
#include <unordered_map>

namespace mge {

    class binder
    {};

    static void resolve_outstanding_binds() {}

    script_context::script_context() { resolve_outstanding_binds(); }
} // namespace mge