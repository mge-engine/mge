// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <memory>

#define MGE_DECLARE_REF(TYPE)                                                  \
    class TYPE;                                                                \
    using TYPE##_ref = std::shared_ptr<TYPE>