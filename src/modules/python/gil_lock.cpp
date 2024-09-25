// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "gil_lock.hpp"

namespace mge::python {

    static gil_lock s_instance;

    gil_lock& gil_lock::instance() { return s_instance; }

} // namespace mge::python