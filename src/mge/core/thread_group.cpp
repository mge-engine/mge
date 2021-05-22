// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/thread_group.hpp"

namespace mge {
    thread_group::thread_group() {}

    thread_group::~thread_group() {}

    thread_group::size_type thread_group::size() const { return 0; }

    thread_group::size_type thread_group::active() const { return 0; }

} // namespace mge