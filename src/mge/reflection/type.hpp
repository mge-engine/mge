// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"

namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT type
        {
        public:
            static template <typename T>
            type<T> get();
        };
}
}
