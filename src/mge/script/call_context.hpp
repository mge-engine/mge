// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/is_shared_ptr.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/script/dllexport.hpp"

#include <functional>
#include <string>
#include <type_traits>
#include <typeindex>

namespace mge::script {

    /**
     * @brief Call context.
     *
     * Used to interface with parameter retrieval by a function invocation.
     *
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        virtual ~call_context();

        /**
         * @brief Retrieve this pointer for current context.
         *
         * @return this pointer, nullpointer if not in a method context
         */
        virtual void* this_ptr() = 0;

        /**
         * @brief Retrieve shared pointer address for current context.
         *
         * @return address of shared pointer, which itself points to this
         * pointer if initialized
         */
        virtual void* shared_ptr_address() = 0;

        virtual bool        bool_parameter(size_t position) = 0;
        virtual int8_t      int8_t_parameter(size_t position) = 0;
        virtual uint8_t     uint8_t_parameter(size_t position) = 0;
        virtual int16_t     int16_t_parameter(size_t position) = 0;
        virtual uint16_t    uint16_t_parameter(size_t position) = 0;
        virtual int32_t     int32_t_parameter(size_t position) = 0;
        virtual uint32_t    uint32_t_parameter(size_t position) = 0;
        virtual int64_t     int64_t_parameter(size_t position) = 0;
        virtual uint64_t    uint64_t_parameter(size_t position) = 0;
        virtual float       float_parameter(size_t position) = 0;
        virtual double      double_parameter(size_t position) = 0;
        virtual std::string string_parameter(size_t position) = 0;

        virtual void* object_parameter(size_t position, std::type_index ti) = 0;

        virtual void store_bool_result(bool result) = 0;
        virtual void store_int8_t_result(int8_t result) = 0;
        virtual void store_uint8_t_result(uint8_t result) = 0;
        virtual void store_int16_t_result(int16_t result) = 0;
        virtual void store_uint16_t_result(uint16_t result) = 0;
        virtual void store_int32_t_result(int32_t result) = 0;
        virtual void store_uint32_t_result(uint32_t result) = 0;
        virtual void store_int64_t_result(int64_t result) = 0;
        virtual void store_uint64_t_result(uint64_t result) = 0;
        virtual void store_float_result(float result) = 0;
        virtual void store_double_result(double result) = 0;
        virtual void store_string_result(const std::string& result) = 0;
    };

} // namespace mge::script