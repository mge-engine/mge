// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "python.hpp"

namespace mge::python {
    class python_call_context : public mge::script::call_context
    {
    public:
        python_call_context(PyObject* args);
        python_call_context(void* this_ptr_, PyObject* args = nullptr);
        ~python_call_context();

        PyObject* result() const { return m_result; }

        void* this_ptr() override;

    protected:
        bool           bool_parameter(size_t position) override;
        char           char_parameter(size_t position) override;
        unsigned char  unsigned_char_parameter(size_t position) override;
        short          short_parameter(size_t position) override;
        unsigned short unsigned_short_parameter(size_t position) override;
        int            int_parameter(size_t position) override;
        unsigned int   unsigned_int_parameter(size_t position) override;
        int64_t        long_parameter(size_t position) override;
        uint64_t       unsigned_long_parameter(size_t position) override;
        float          float_parameter(size_t position) override;
        double         double_parameter(size_t position) override;
        std::string    string_parameter(size_t position) override;
        void* object_parameter(size_t position, std::type_index type) override;

        void store_bool_result(bool result) override;
        void store_char_result(char result) override;
        void store_signed_char_result(signed char result) override;
        void store_unsigned_char_result(unsigned char result) override;
        void store_short_result(short result) override;
        void store_unsigned_short_result(unsigned short result) override;
        void store_int_result(int result) override;
        void store_unsigned_int_result(unsigned int result) override;
        void store_long_result(int64_t result) override;
        void store_unsigned_long_result(uint64_t result) override;
        void store_float_result(float result) override;
        void store_double_result(double result) override;
        void store_string_result(const std::string& result) override;

    private:
        PyObject* parameter(size_t position);
        void      store(PyObject* object);

        void*     m_this_ptr;
        PyObject* m_args;
        PyObject* m_result;
    };
} // namespace mge::python