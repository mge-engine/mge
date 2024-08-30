// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "python.hpp"

namespace mge::python {

    class python_type;

    class python_object_call_context : public mge::script::call_context
    {
    public:
        python_object_call_context(python_type* type,
                                   PyObject*    self,
                                   PyObject*    args = nullptr,
                                   size_t       offset = 0);
        ~python_object_call_context();

        void* this_ptr() override;
        void* shared_ptr_address() override;

        bool        bool_parameter(size_t position) override;
        int8_t      int8_t_parameter(size_t position) override;
        uint8_t     uint8_t_parameter(size_t position) override;
        int16_t     int16_t_parameter(size_t position) override;
        uint16_t    uint16_t_parameter(size_t position) override;
        int32_t     int32_t_parameter(size_t position) override;
        uint32_t    uint32_t_parameter(size_t position) override;
        int64_t     int64_t_parameter(size_t position) override;
        uint64_t    uint64_t_parameter(size_t position) override;
        float       float_parameter(size_t position) override;
        double      double_parameter(size_t position) override;
        std::string string_parameter(size_t position) override;

        void* object_parameter(size_t position, std::type_index ti) override;

        void store_bool_result(bool result) override;
        void store_int8_t_result(int8_t result) override;
        void store_uint8_t_result(uint8_t result) override;
        void store_int16_t_result(int16_t result) override;
        void store_uint16_t_result(uint16_t result) override;
        void store_int32_t_result(int32_t result) override;
        void store_uint32_t_result(uint32_t result) override;
        void store_int64_t_result(int64_t result) override;
        void store_uint64_t_result(uint64_t result) override;
        void store_float_result(float result) override;
        void store_double_result(double result) override;
        void store_string_result(const std::string& result) override;
        void store_shared_ptr_result(const std::type_index& type,
                                     const void*            address) override;

        PyObject* result() const;

    private:
        PyObject* arg(size_t position);

        python_type* m_type;
        PyObject*    m_self;
        PyObject*    m_args;
        size_t       m_offset;
        PyObject*    m_result;
        bool         m_args_tuple;
        mutable bool m_result_returned;
    };
} // namespace mge::python