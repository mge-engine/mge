// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "python_call_context.hpp"
#include "python_error.hpp"
#include "python_instance.hpp"
#include "python_type.hpp"

#include "mge/core/stdexceptions.hpp"
#include "mge/reflection/type_details.hpp"

#include <unordered_map>

namespace mge::python {

    python_call_context::python_call_context(PyObject* args,
                                             int       param_start,
                                             void*     this_ptr)
        : m_args(args)
        , m_param_start(param_start)
        , m_this_ptr(this_ptr)
    {}

    python_call_context::~python_call_context()
    {
        Py_XDECREF(m_result);
    }

    PyObject* python_call_context::arg_at(size_t index)
    {
        return PyTuple_GetItem(m_args, m_param_start + static_cast<int>(index));
    }

    void* python_call_context::this_ptr()
    {
        return m_this_ptr;
    }

    bool python_call_context::bool_parameter(size_t index)
    {
        return PyObject_IsTrue(arg_at(index)) != 0;
    }

    int8_t python_call_context::int8_t_parameter(size_t index)
    {
        return static_cast<int8_t>(PyLong_AsLong(arg_at(index)));
    }

    uint8_t python_call_context::uint8_t_parameter(size_t index)
    {
        return static_cast<uint8_t>(PyLong_AsUnsignedLong(arg_at(index)));
    }

    int16_t python_call_context::int16_t_parameter(size_t index)
    {
        return static_cast<int16_t>(PyLong_AsLong(arg_at(index)));
    }

    uint16_t python_call_context::uint16_t_parameter(size_t index)
    {
        return static_cast<uint16_t>(PyLong_AsUnsignedLong(arg_at(index)));
    }

    int32_t python_call_context::int32_t_parameter(size_t index)
    {
        return static_cast<int32_t>(PyLong_AsLong(arg_at(index)));
    }

    uint32_t python_call_context::uint32_t_parameter(size_t index)
    {
        return static_cast<uint32_t>(PyLong_AsUnsignedLong(arg_at(index)));
    }

    int64_t python_call_context::int64_t_parameter(size_t index)
    {
        return static_cast<int64_t>(PyLong_AsLongLong(arg_at(index)));
    }

    uint64_t python_call_context::uint64_t_parameter(size_t index)
    {
        return static_cast<uint64_t>(PyLong_AsUnsignedLongLong(arg_at(index)));
    }

    float python_call_context::float_parameter(size_t index)
    {
        return static_cast<float>(PyFloat_AsDouble(arg_at(index)));
    }

    double python_call_context::double_parameter(size_t index)
    {
        return PyFloat_AsDouble(arg_at(index));
    }

    long double python_call_context::long_double_parameter(size_t index)
    {
        return static_cast<long double>(PyFloat_AsDouble(arg_at(index)));
    }

    std::string_view python_call_context::string_view_parameter(size_t index)
    {
        PyObject*   arg = arg_at(index);
        Py_ssize_t  len = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(arg, &len);
        if (!utf8) {
            return {};
        }
        m_string_storage.emplace_back(utf8, static_cast<size_t>(len));
        return m_string_storage.back();
    }

    void* python_call_context::pointer_parameter(
        size_t index, const mge::reflection::type_details& details)
    {
        PyObject* arg = arg_at(index);
        if (!arg || arg == Py_None) {
            return nullptr;
        }

        // Python str passed for const std::string& parameter: convert and pin
        // storage so the view returned by the caller stays valid for the call.
        if (PyUnicode_Check(arg)) {
            const auto& std_string_td =
                mge::reflection::type_details::get<std::string>();
            if (std_string_td && std_string_td.get() == &details) {
                Py_ssize_t  len  = 0;
                const char* utf8 = PyUnicode_AsUTF8AndSize(arg, &len);
                if (utf8) {
                    m_string_storage.emplace_back(utf8,
                                                   static_cast<size_t>(len));
                    return &m_string_storage.back();
                }
                return nullptr;
            }
        }

        PyObject* capsule = PyObject_GetAttrString(
            reinterpret_cast<PyObject*>(Py_TYPE(arg)), "__mge_type_details__");
        if (capsule) {
            PyErr_Clear();
            if (PyCapsule_CheckExact(capsule)) {
                Py_DECREF(capsule);
                auto* h = reinterpret_cast<python_instance_header*>(arg);
                return h->object.get();
            }
            Py_DECREF(capsule);
        } else {
            PyErr_Clear();
        }
        return nullptr;
    }

    std::unordered_map<std::type_index,
                        python_call_context::callable_factory_fn>&
    python_call_context::callable_factories()
    {
        static std::unordered_map<std::type_index, callable_factory_fn>
            s_factories;
        return s_factories;
    }

    void* python_call_context::callable_parameter(size_t                 index,
                                                   const std::type_index& callable_type)
    {
        auto it = callable_factories().find(callable_type);
        if (it == callable_factories().end()) {
            MGE_THROW_NOT_IMPLEMENTED
                << "Python callable type not registered: " << callable_type.name();
        }
        PyObject* py_callable = arg_at(index);
        if (!py_callable || !PyCallable_Check(py_callable)) {
            PyErr_SetString(PyExc_TypeError, "expected callable");
            MGE_THROW(mge::python::error) << "expected callable argument";
        }
        auto shared = it->second(py_callable);
        m_callable_storage.push_back(shared);
        return shared.get();
    }

    void python_call_context::bool_result(bool value)
    {
        Py_XDECREF(m_result);
        m_result = PyBool_FromLong(value ? 1 : 0);
    }

    void python_call_context::int8_t_result(int8_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromLong(value);
    }

    void python_call_context::uint8_t_result(uint8_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromUnsignedLong(value);
    }

    void python_call_context::int16_t_result(int16_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromLong(value);
    }

    void python_call_context::uint16_t_result(uint16_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromUnsignedLong(value);
    }

    void python_call_context::int32_t_result(int32_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromLong(value);
    }

    void python_call_context::uint32_t_result(uint32_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromUnsignedLong(value);
    }

    void python_call_context::int64_t_result(int64_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromLongLong(value);
    }

    void python_call_context::uint64_t_result(uint64_t value)
    {
        Py_XDECREF(m_result);
        m_result = PyLong_FromUnsignedLongLong(value);
    }

    void python_call_context::float_result(float value)
    {
        Py_XDECREF(m_result);
        m_result = PyFloat_FromDouble(static_cast<double>(value));
    }

    void python_call_context::double_result(double value)
    {
        Py_XDECREF(m_result);
        m_result = PyFloat_FromDouble(value);
    }

    void python_call_context::long_double_result(long double value)
    {
        Py_XDECREF(m_result);
        m_result = PyFloat_FromDouble(static_cast<double>(value));
    }

    void python_call_context::string_view_result(std::string_view value)
    {
        Py_XDECREF(m_result);
        m_result = PyUnicode_FromStringAndSize(value.data(),
                                               static_cast<Py_ssize_t>(value.size()));
    }

    void python_call_context::pointer_result(void* value)
    {
        Py_XDECREF(m_result);
        if (m_pointer_result_type && value) {
            m_result = python_type::create_foreign_instance(m_pointer_result_type, value);
        } else if (value) {
            m_result = PyCapsule_New(value, nullptr, nullptr);
        } else {
            Py_INCREF(Py_None);
            m_result = Py_None;
        }
    }

    void python_call_context::shared_ptr_result(std::shared_ptr<void> value)
    {
        Py_XDECREF(m_result);
        if (m_shared_ptr_result_type && value) {
            m_result = python_type::create_shared_instance(m_shared_ptr_result_type,
                                                           std::move(value));
        } else {
            Py_INCREF(Py_None);
            m_result = Py_None;
        }
    }

    void python_call_context::primitive_vector_result(const void*            data,
                                                       size_t                 count,
                                                       const std::type_index& element_type)
    {
        Py_XDECREF(m_result);
        PyObject* list = PyList_New(static_cast<Py_ssize_t>(count));
        if (!list) {
            return;
        }
        for (size_t i = 0; i < count; ++i) {
            PyObject* item = nullptr;
            if (element_type == std::type_index(typeid(bool))) {
                item = PyBool_FromLong(static_cast<const bool*>(data)[i] ? 1 : 0);
            } else if (element_type == std::type_index(typeid(int8_t))) {
                item = PyLong_FromLong(static_cast<const int8_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint8_t))) {
                item = PyLong_FromUnsignedLong(static_cast<const uint8_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int16_t))) {
                item = PyLong_FromLong(static_cast<const int16_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint16_t))) {
                item = PyLong_FromUnsignedLong(static_cast<const uint16_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int32_t))) {
                item = PyLong_FromLong(static_cast<const int32_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint32_t))) {
                item = PyLong_FromUnsignedLong(static_cast<const uint32_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(int64_t))) {
                item = PyLong_FromLongLong(static_cast<const int64_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(uint64_t))) {
                item = PyLong_FromUnsignedLongLong(static_cast<const uint64_t*>(data)[i]);
            } else if (element_type == std::type_index(typeid(long))) {
                item = PyLong_FromLong(static_cast<const long*>(data)[i]);
            } else if (element_type == std::type_index(typeid(unsigned long))) {
                item = PyLong_FromUnsignedLong(
                    static_cast<const unsigned long*>(data)[i]);
            } else if (element_type == std::type_index(typeid(float))) {
                item = PyFloat_FromDouble(
                    static_cast<double>(static_cast<const float*>(data)[i]));
            } else if (element_type == std::type_index(typeid(double))) {
                item = PyFloat_FromDouble(static_cast<const double*>(data)[i]);
            } else if (element_type == std::type_index(typeid(long double))) {
                item = PyFloat_FromDouble(static_cast<double>(
                    static_cast<const long double*>(data)[i]));
            } else if (element_type == std::type_index(typeid(std::string))) {
                const auto& s = static_cast<const std::string*>(data)[i];
                item = PyUnicode_FromStringAndSize(s.data(),
                                                   static_cast<Py_ssize_t>(s.size()));
            } else if (element_type == std::type_index(typeid(std::string_view))) {
                const auto& sv = static_cast<const std::string_view*>(data)[i];
                item = PyUnicode_FromStringAndSize(sv.data(),
                                                   static_cast<Py_ssize_t>(sv.size()));
            } else {
                Py_INCREF(Py_None);
                item = Py_None;
            }
            PyList_SET_ITEM(list, static_cast<Py_ssize_t>(i), item);
        }
        m_result = list;
    }

    void python_call_context::primitive_vector_parameter(
        size_t index, void* out_vector, const std::type_index& element_type)
    {
        PyObject*  arg = arg_at(index);
        Py_ssize_t n = 0;
        bool       is_list = PyList_Check(arg) != 0;
        if (is_list) {
            n = PyList_Size(arg);
        } else if (PyTuple_Check(arg) != 0) {
            n = PyTuple_Size(arg);
        }

        for (Py_ssize_t i = 0; i < n; ++i) {
            PyObject* item =
                is_list ? PyList_GET_ITEM(arg, i) : PyTuple_GET_ITEM(arg, i);

            if (element_type == std::type_index(typeid(bool))) {
                static_cast<std::vector<bool>*>(out_vector)->push_back(
                    PyObject_IsTrue(item) != 0);
            } else if (element_type == std::type_index(typeid(int8_t))) {
                static_cast<std::vector<int8_t>*>(out_vector)->push_back(
                    static_cast<int8_t>(PyLong_AsLong(item)));
            } else if (element_type == std::type_index(typeid(uint8_t))) {
                static_cast<std::vector<uint8_t>*>(out_vector)->push_back(
                    static_cast<uint8_t>(PyLong_AsUnsignedLong(item)));
            } else if (element_type == std::type_index(typeid(int16_t))) {
                static_cast<std::vector<int16_t>*>(out_vector)->push_back(
                    static_cast<int16_t>(PyLong_AsLong(item)));
            } else if (element_type == std::type_index(typeid(uint16_t))) {
                static_cast<std::vector<uint16_t>*>(out_vector)->push_back(
                    static_cast<uint16_t>(PyLong_AsUnsignedLong(item)));
            } else if (element_type == std::type_index(typeid(int32_t))) {
                static_cast<std::vector<int32_t>*>(out_vector)->push_back(
                    static_cast<int32_t>(PyLong_AsLong(item)));
            } else if (element_type == std::type_index(typeid(uint32_t))) {
                static_cast<std::vector<uint32_t>*>(out_vector)->push_back(
                    static_cast<uint32_t>(PyLong_AsUnsignedLong(item)));
            } else if (element_type == std::type_index(typeid(int64_t))) {
                static_cast<std::vector<int64_t>*>(out_vector)->push_back(
                    static_cast<int64_t>(PyLong_AsLongLong(item)));
            } else if (element_type == std::type_index(typeid(uint64_t))) {
                static_cast<std::vector<uint64_t>*>(out_vector)->push_back(
                    static_cast<uint64_t>(PyLong_AsUnsignedLongLong(item)));
            } else if (element_type == std::type_index(typeid(long))) {
                static_cast<std::vector<long>*>(out_vector)->push_back(
                    PyLong_AsLong(item));
            } else if (element_type == std::type_index(typeid(unsigned long))) {
                static_cast<std::vector<unsigned long>*>(out_vector)->push_back(
                    PyLong_AsUnsignedLong(item));
            } else if (element_type == std::type_index(typeid(float))) {
                static_cast<std::vector<float>*>(out_vector)->push_back(
                    static_cast<float>(PyFloat_AsDouble(item)));
            } else if (element_type == std::type_index(typeid(double))) {
                static_cast<std::vector<double>*>(out_vector)->push_back(
                    PyFloat_AsDouble(item));
            } else if (element_type == std::type_index(typeid(long double))) {
                static_cast<std::vector<long double>*>(out_vector)->push_back(
                    static_cast<long double>(PyFloat_AsDouble(item)));
            } else if (element_type == std::type_index(typeid(std::string))) {
                Py_ssize_t  len = 0;
                const char* utf8 = PyUnicode_AsUTF8AndSize(item, &len);
                if (utf8) {
                    static_cast<std::vector<std::string>*>(out_vector)->emplace_back(
                        utf8, static_cast<size_t>(len));
                }
            } else if (element_type == std::type_index(typeid(std::string_view))) {
                Py_ssize_t  len = 0;
                const char* utf8 = PyUnicode_AsUTF8AndSize(item, &len);
                if (utf8) {
                    static_cast<std::vector<std::string_view>*>(out_vector)->emplace_back(
                        utf8, static_cast<size_t>(len));
                }
            }
        }
    }

    void python_call_context::exception_thrown(const mge::exception& ex)
    {
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        m_exception = true;
    }

    void python_call_context::exception_thrown(const std::exception& ex)
    {
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        m_exception = true;
    }

    void python_call_context::exception_thrown()
    {
        PyErr_SetString(PyExc_RuntimeError, "unknown C++ exception");
        m_exception = true;
    }

    PyObject* python_call_context::result()
    {
        if (m_exception) {
            return nullptr;
        }
        PyObject* r = m_result;
        m_result = nullptr;
        if (!r) {
            Py_INCREF(Py_None);
            r = Py_None;
        }
        return r;
    }

} // namespace mge::python
