// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

namespace mge::reflection {

    /**
     * Context of a call. When a function/method is called by reflection,
     * a call context is passed which implements callbacks to pass parameter
     * values and receive results.
     */
    class MGEREFLECTION_EXPORT call_context
    {
    protected:
        call_context() = default;

    public:
        ~call_context() = default;

        /**
         * Pointer to current object.
         * @return pointer to current object, nullptr for static methods or
         * object construction
         */
        virtual void* this_ptr() = 0;

        virtual void bool_result(bool value) = 0;
        virtual void int8_result(int8_t value) = 0;
        virtual void uint8_result(uint8_t value) = 0;
        virtual void int16_result(int16_t value) = 0;
        virtual void uint16_result(uint16_t value) = 0;
        virtual void int32_result(int32_t value) = 0;
        virtual void uint32_result(uint32_t value) = 0;
        virtual void int64_result(int64_t value) = 0;
        virtual void uint64_result(uint64_t value) = 0;
        virtual void float_result(float value) = 0;
        virtual void double_result(double value) = 0;
    
    };

} // namespace mge::reflection