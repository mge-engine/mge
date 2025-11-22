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
        call_context();

    public:
        virtual ~call_context();

        /**
         * Pointer to current object.
         * @return pointer to current object, nullptr for static methods
         */
        virtual void* this_ptr() = 0;

        virtual void bool_result(bool value) = 0;
        virtual void int8_t_result(int8_t value) = 0;
        virtual void uint8_t_result(uint8_t value) = 0;
        virtual void int16_t_result(int16_t value) = 0;
        virtual void uint16_t_result(uint16_t value) = 0;
        virtual void int32_t_result(int32_t value) = 0;
        virtual void uint32_t_result(uint32_t value) = 0;
        virtual void int64_t_result(int64_t value) = 0;
        virtual void uint64_t_result(uint64_t value) = 0;
        virtual void float_result(float value) = 0;
        virtual void double_result(double value) = 0;
    };

} // namespace mge::reflection