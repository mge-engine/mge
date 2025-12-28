// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace mge {
    using buffer = std::vector<uint8_t>;
    using buffer_ref = std::shared_ptr<buffer>;
} // namespace mge