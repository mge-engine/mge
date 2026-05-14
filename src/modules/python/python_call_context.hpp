// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/call_context.hpp"
#include "python.hpp"
#include "python_fwd.hpp"
#include "pyobject_ref.hpp"

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace mge::python {

    class python_call_context : public mge::reflection::call_context
    {
    public:
        python_call_context(PyObject* args, int param_start, void* this_ptr);
        ~python_call_context() override;

        bool             bool_parameter(size_t index) override;
        int8_t           int8_t_parameter(size_t index) override;
        uint8_t          uint8_t_parameter(size_t index) override;
        int16_t          int16_t_parameter(size_t index) override;
        uint16_t         uint16_t_parameter(size_t index) override;
        int32_t          int32_t_parameter(size_t index) override;
        uint32_t         uint32_t_parameter(size_t index) override;
        int64_t          int64_t_parameter(size_t index) override;
        uint64_t         uint64_t_parameter(size_t index) override;
        float            float_parameter(size_t index) override;
        double           double_parameter(size_t index) override;
        long double      long_double_parameter(size_t index) override;
        std::string_view string_view_parameter(size_t index) override;
        void*            pointer_parameter(
                             size_t                               index,
                             const mge::reflection::type_details& details) override;
        void* callable_parameter(size_t                 index,
                                 const std::type_index& callable_type) override;
        void* this_ptr() override;

        void bool_result(bool value) override;
        void int8_t_result(int8_t value) override;
        void uint8_t_result(uint8_t value) override;
        void int16_t_result(int16_t value) override;
        void uint16_t_result(uint16_t value) override;
        void int32_t_result(int32_t value) override;
        void uint32_t_result(uint32_t value) override;
        void int64_t_result(int64_t value) override;
        void uint64_t_result(uint64_t value) override;
        void float_result(float value) override;
        void double_result(double value) override;
        void long_double_result(long double value) override;
        void string_view_result(std::string_view value) override;
        void pointer_result(void* value) override;
        void shared_ptr_result(std::shared_ptr<void> value) override;
        void primitive_vector_result(const void*            data,
                                     size_t                 count,
                                     const std::type_index& element_type) override;
        void primitive_vector_parameter(
            size_t                 index,
            void*                  out_vector,
            const std::type_index& element_type) override;

        void exception_thrown(const mge::exception& ex) override;
        void exception_thrown(const std::exception& ex) override;
        void exception_thrown() override;

        PyObject* result();
        bool      had_exception() const { return m_exception; }

        void set_pointer_result_type(const mge::reflection::type_details* type)
        {
            m_pointer_result_type = type;
        }

        void set_shared_ptr_result_type(const mge::reflection::type_details* type)
        {
            m_shared_ptr_result_type = type;
        }

        using callable_factory_fn =
            std::function<std::shared_ptr<void>(PyObject*)>;

        static std::unordered_map<std::type_index, callable_factory_fn>&
        callable_factories();

        template <typename T>
        static PyObject* cpp_to_python_arg(T val)
        {
            using bare = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<bare, bool>) {
                return PyBool_FromLong(val ? 1 : 0);
            } else if constexpr (std::is_same_v<bare, float> ||
                                 std::is_same_v<bare, double> ||
                                 std::is_same_v<bare, long double>) {
                return PyFloat_FromDouble(static_cast<double>(val));
            } else if constexpr (std::is_unsigned_v<bare>) {
                return PyLong_FromUnsignedLongLong(
                    static_cast<unsigned long long>(val));
            } else if constexpr (std::is_integral_v<bare>) {
                return PyLong_FromLongLong(static_cast<long long>(val));
            } else {
                Py_INCREF(Py_None);
                return Py_None;
            }
        }

        template <typename R, typename... Args>
        static void register_callable_type()
        {
            callable_factories()[std::type_index(
                typeid(std::function<R(Args...)>))] =
                [](PyObject* py_callable) -> std::shared_ptr<void> {
                auto ref = std::make_shared<pyobject_ref>(
                    py_callable, pyobject_ref::incref::yes);
                return std::make_shared<std::function<R(Args...)>>(
                    [ref](Args... args) -> R {
                        PyGILState_STATE g = PyGILState_Ensure();
                        PyObject* tup =
                            PyTuple_New(static_cast<Py_ssize_t>(sizeof...(Args)));
                        size_t i = 0;
                        (PyTuple_SET_ITEM(tup,
                                          static_cast<Py_ssize_t>(i++),
                                          cpp_to_python_arg(args)),
                         ...);
                        PyObject* r =
                            PyObject_Call(ref->get(), tup, nullptr);
                        Py_DECREF(tup);
                        if (!r) {
                            PyErr_Clear();
                            PyGILState_Release(g);
                            if constexpr (!std::is_void_v<R>) {
                                return R{};
                            } else {
                                return;
                            }
                        }
                        if constexpr (!std::is_void_v<R>) {
                            R result{};
                            Py_DECREF(r);
                            PyGILState_Release(g);
                            return result;
                        } else {
                            Py_DECREF(r);
                            PyGILState_Release(g);
                        }
                    });
            };
        }

    private:
        PyObject* arg_at(size_t index);

        PyObject* m_args;
        int       m_param_start;
        void*     m_this_ptr;
        PyObject* m_result{nullptr};
        bool      m_exception{false};

        const mge::reflection::type_details* m_pointer_result_type{nullptr};
        const mge::reflection::type_details* m_shared_ptr_result_type{nullptr};

        std::vector<std::string>           m_string_storage;
        std::vector<std::shared_ptr<void>> m_callable_storage;
    };

} // namespace mge::python
