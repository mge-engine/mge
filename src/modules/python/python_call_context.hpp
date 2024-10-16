#pragma once
#include "mge/script/call_context.hpp"
#include "python.hpp"

namespace mge::python {

    class python_call_context : public mge::script::call_context
    {
    public:
        python_call_context(void* this_ptr_, void* shared_ptr_address_)
            : m_this_ptr(this_ptr_)
            , m_shared_ptr_address(shared_ptr_address_)
        {}

        ~python_call_context() = default;

        void* this_ptr() override { return m_this_ptr; }
        void* shared_ptr_address() override { return m_shared_ptr_address; }

        void bool_result(bool result) override;

        void int8_t_result(int8_t result) override;
        void uint8_t_result(uint8_t result) override;
        void int16_t_result(int16_t result) override;
        void uint16_t_result(uint16_t result) override;
        void int32_t_result(int32_t result) override;
        void uint32_t_result(uint32_t result) override;
        void int64_t_result(int64_t result) override;
        void uint64_t_result(uint64_t result) override;

        void float_result(float result) override;
        void double_result(double result) override;
        void long_double_result(long double result) override;

        void string_result(const std::string& result) override;
        void wstring_result(const std::wstring& result) override;
        void object_result(void*                             result,
                           const mge::script::type_data_ref& t) override;

        void exception_thrown(const std::exception& e) override;
        void exception_thrown(const mge::exception& e) override;
        void exception_thrown() override;

        bool     bool_parameter(size_t position) override;
        int8_t   int8_t_parameter(size_t position) override;
        uint8_t  uint8_t_parameter(size_t position) override;
        int16_t  int16_t_parameter(size_t position) override;
        uint16_t uint16_t_parameter(size_t position) override;
        int32_t  int32_t_parameter(size_t position) override;

        uint32_t uint32_t_parameter(size_t position) override;

        int64_t     int64_t_parameter(size_t position) override;
        uint64_t    uint64_t_parameter(size_t position) override;
        float       float_parameter(size_t position) override;
        double      double_parameter(size_t position) override;
        long double long_double_parameter(size_t position) override;

        std::string string_parameter(size_t position) override;

        void before_call() override {}
        void after_call() override {}

        void set_arguments(PyObject* args)
        {
            m_arguments = args;
            m_is_single_argument = false;
        }

        void set_single_argument(PyObject* arg)
        {
            m_arguments = arg;
            m_is_single_argument = true;
        }

        PyObject* result() { return m_result; }
        bool      has_exception() const { return m_has_exception; }

    private:
        PyObject* argument(size_t position);

        void*     m_this_ptr;
        void*     m_shared_ptr_address;
        PyObject* m_arguments{nullptr};
        PyObject* m_result{nullptr};
        bool      m_has_exception{false};
        bool      m_is_single_argument{false};
    };
} // namespace mge::python