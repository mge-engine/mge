// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
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

        virtual bool             bool_parameter(size_t index) = 0;
        virtual int8_t           int8_t_parameter(size_t index) = 0;
        virtual uint8_t          uint8_t_parameter(size_t index) = 0;
        virtual int16_t          int16_t_parameter(size_t index) = 0;
        virtual uint16_t         uint16_t_parameter(size_t index) = 0;
        virtual int32_t          int32_t_parameter(size_t index) = 0;
        virtual uint32_t         uint32_t_parameter(size_t index) = 0;
        virtual int64_t          int64_t_parameter(size_t index) = 0;
        virtual uint64_t         uint64_t_parameter(size_t index) = 0;
        virtual float            float_parameter(size_t index) = 0;
        virtual double           double_parameter(size_t index) = 0;
        virtual long double      long_double_parameter(size_t index) = 0;
        virtual std::string_view string_view_parameter(size_t index) = 0;
        virtual void*            pointer_parameter(size_t              index,
                                                   const type_details& details) = 0;

        template <typename T>
            requires std::is_pointer_v<T>
        T pointer_parameter(size_t index, const type_details& details)
        {
            return static_cast<T>(pointer_parameter(index, details));
        }

        template <typename T>
            requires(!std::is_pointer_v<T>)
        T parameter(size_t index)
        {
            if constexpr (std::is_reference_v<T>) {
                MGE_THROW_NOT_IMPLEMENTED << "Reference type not supported yet";
            } else if constexpr (std::is_enum_v<T>) {
                using UT = std::underlying_type_t<T>;
                return static_cast<T>(parameter<UT>(index));
            } else if constexpr (std::is_same_v<T, bool>) {
                return bool_parameter(index);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                return int8_t_parameter(index);
            } else if constexpr (std::is_same_v<T, uint8_t>) {
                return uint8_t_parameter(index);
            } else if constexpr (std::is_same_v<T, int16_t>) {
                return int16_t_parameter(index);
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                return uint16_t_parameter(index);
            } else if constexpr (std::is_same_v<T, int32_t>) {
                return int32_t_parameter(index);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return uint32_t_parameter(index);
            } else if constexpr (std::is_same_v<T, int64_t>) {
                return int64_t_parameter(index);
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                return uint64_t_parameter(index);
            } else if constexpr (std::is_same_v<T, float>) {
                return float_parameter(index);
            } else if constexpr (std::is_same_v<T, double>) {
                return double_parameter(index);
            } else if constexpr (std::is_same_v<T, long double>) {
                return long_double_parameter(index);
            } else if constexpr (std::is_same_v<T, std::string_view>) {
                return string_view_parameter(index);
            }
        }

        /**
         * Pointer to current object.
         * @return pointer to current object, nullptr for static methods
         */
        virtual void* this_ptr() = 0;

        /**
         * Reference to current object.
         * @return reference to current object
         * @throws mge::null_pointer if there is no object
         */
        template <typename T> T& this_reference()
        {
            void* ptr = this_ptr();
            if (ptr == nullptr) {
                MGE_THROW(null_pointer) << "this_ptr() is nullptr";
            }
            return *static_cast<T*>(ptr);
        }

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
        virtual void long_double_result(long double value) = 0;
        virtual void string_view_result(std::string_view value) = 0;
        virtual void pointer_result(void* value) = 0;

        template <typename T> void result(T value)
        {
            if constexpr (std::is_pointer_v<T>) {
                pointer_result(static_cast<void*>(value));
            } else if constexpr (std::is_enum_v<T>) {
                using UT = std::underlying_type_t<T>;
                result(static_cast<UT>(value));
            } else if constexpr (std::is_same_v<T, bool>) {
                bool_result(value);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                int8_t_result(value);
            } else if constexpr (std::is_same_v<T, uint8_t>) {
                uint8_t_result(value);
            } else if constexpr (std::is_same_v<T, int16_t>) {
                int16_t_result(value);
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                uint16_t_result(value);
            } else if constexpr (std::is_same_v<T, int32_t>) {
                int32_t_result(value);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                uint32_t_result(value);
            } else if constexpr (std::is_same_v<T, int64_t>) {
                int64_t_result(value);
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                uint64_t_result(value);
            } else if constexpr (std::is_same_v<T, float>) {
                float_result(value);
            } else if constexpr (std::is_same_v<T, double>) {
                double_result(value);
            } else if constexpr (std::is_same_v<T, long double>) {
                long_double_result(value);
            } else if constexpr (std::is_same_v<T, std::string_view>) {
                string_view_result(value);
            }
        }

        virtual void exception_thrown(const mge::exception& ex) = 0;
        virtual void exception_thrown(const std::exception& ex) = 0;
        virtual void exception_thrown() = 0;
    };

} // namespace mge::reflection