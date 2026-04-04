// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    class MGEREFLECTION_EXPORT visitor
    {
    public:
        virtual ~visitor() = default;

        virtual void before(const module_details& details) = 0;
        virtual void on(const module_details& details) = 0;
        virtual void after(const module_details& details) = 0;

        virtual void before(const type_details& details) = 0;
        virtual void on(const type_details& details) = 0;
        virtual void after(const type_details& details) = 0;

        virtual void before(const function_details& details) = 0;
        virtual void on(const function_details& details) = 0;
        virtual void after(const function_details& details) = 0;
    };

} // namespace mge::reflection