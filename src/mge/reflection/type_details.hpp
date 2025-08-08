// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    class MGEREFLECTION_EXPORT type_details
    {
    public:
        type_details();
        ~type_details();

        type_details(const type_details&) = delete;
        type_details& operator=(const type_details&) = delete;
        type_details(type_details&&) = delete;
        type_details& operator=(type_details&&) = delete;
    };

} // namespace mge::reflection
