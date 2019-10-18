// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include <iosfwd>
namespace dx12 {
    std::ostream& operator <<(std::ostream& os, const D3D_FEATURE_LEVEL& l);
}