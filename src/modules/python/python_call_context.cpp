#include "python_call_context.hpp"
#include "mge/core/checked_cast.hpp"
#include "python_error.hpp"

namespace mge::python {

    python_call_context::python_call_context(PyObject* args)
        : m_args(args)
        , m_result(nullptr)
    {
        if (args) {
            Py_INCREF(m_args);
        }
        m_result = Py_None;
        Py_INCREF(m_result);
    }

    python_call_context::~python_call_context()
    {
        Py_CLEAR(m_args);
        Py_CLEAR(m_result);
    }

    void* python_call_context::this_ptr() { return nullptr; }

    bool python_call_context::bool_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        int       rc = PyObject_IsTrue(val);
        switch (rc) {
        case 1:
            return true;
        case 0:
            return false;
        case -1:
            error::check_error();
        }
        return false;
    }

    char python_call_context::char_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<char>(l);
    }

    unsigned char python_call_context::unsigned_char_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsUnsignedLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<unsigned char>(l);
    }

    short python_call_context::short_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<short>(l);
    }

    unsigned short
    python_call_context::unsigned_short_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsUnsignedLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<unsigned short>(l);
    }

    int python_call_context::int_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<int>(l);
    }

    unsigned int python_call_context::unsigned_int_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsUnsignedLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<unsigned int>(l);
    }

    int64_t python_call_context::long_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsLongLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<int64_t>(l);
    }

    uint64_t python_call_context::unsigned_long_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      l = PyLong_AsUnsignedLongLong(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<uint64_t>(l);
    }

    float python_call_context::float_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      f = PyFloat_AsDouble(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return checked_cast<float>(f);
    }

    double python_call_context::double_parameter(size_t position)
    {
        PyObject* val = parameter(position);
        auto      f = PyFloat_AsDouble(val);
        if (PyErr_Occurred()) {
            error::check_error();
        }
        return f;
    }

    std::string python_call_context::string_parameter(size_t position)
    {
        PyObject*   val = parameter(position);
        Py_ssize_t  sz = 0;
        const char* utf8 = PyUnicode_AsUTF8AndSize(val, &sz);
        if (PyErr_Occurred() || utf8 == nullptr) {
            error::check_error();
        }
        return std::string(utf8, utf8 + sz);
    }

    PyObject* python_call_context::parameter(size_t position)
    {
        auto result =
            PyTuple_GetItem(m_args, checked_cast<Py_ssize_t>(position));
        if (!result) {
            error::check_error();
        }
        return result;
    }

    void python_call_context::store(PyObject* object)
    {
        Py_CLEAR(m_result);
        m_result = object;
        Py_INCREF(m_result);
    }

    void python_call_context::store_bool_result(bool result)
    {
        if (result) {
            store(Py_True);
        } else {
            store(Py_False);
        }
    }

    void python_call_context::store_char_result(char result)
    {
        auto obj = Py_BuildValue("b", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_signed_char_result(signed char result)
    {
        auto obj = Py_BuildValue("b", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_unsigned_char_result(unsigned char result)
    {
        auto obj = Py_BuildValue("B", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_short_result(short result)
    {
        auto obj = Py_BuildValue("h", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_unsigned_short_result(unsigned short result)
    {
        auto obj = Py_BuildValue("H", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_int_result(int result)
    {
        auto obj = Py_BuildValue("i", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_unsigned_int_result(unsigned int result)
    {
        auto obj = Py_BuildValue("I", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_long_result(int64_t result)
    {
        auto obj = Py_BuildValue("L", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_unsigned_long_result(uint64_t result)
    {
        auto obj = Py_BuildValue("K", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_float_result(float result)
    {
        auto obj = Py_BuildValue("f", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_double_result(double result)
    {
        auto obj = Py_BuildValue("d", result);
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

    void python_call_context::store_string_result(const std::string& result)
    {
        auto obj = Py_BuildValue("s", result.c_str());
        if (!obj) {
            error::check_error();
        }
        store(obj);
        Py_CLEAR(obj);
    }

} // namespace mge::python