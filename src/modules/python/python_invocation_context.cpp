#include "python_invocation_context.hpp"
#include "python_error.hpp"
#include "python_type.hpp"

#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_invocation_context::python_invocation_context(
        const python_type& type, PyObject* self)
        : m_type(type)
        , m_self(self)
        , m_result(nullptr)
    {
        // MGE_DEBUG_TRACE(PYTHON)
        //     << "python_invocation_context@" << this << " ctor";
    }

    python_invocation_context::~python_invocation_context()
    {
        // MGE_DEBUG_TRACE(PYTHON)
        //     << "python_invocation_context@" << this << " dtor";
    }

    void python_invocation_context::store_bool_argument(size_t index,
                                                        bool   value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_int8_t_argument(size_t index,
                                                          int8_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_uint8_t_argument(size_t  index,
                                                           uint8_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_int16_t_argument(size_t  index,
                                                           int16_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_uint16_t_argument(size_t   index,
                                                            uint16_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_int32_t_argument(size_t  index,
                                                           int32_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_uint32_t_argument(size_t   index,
                                                            uint32_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_int64_t_argument(size_t  index,
                                                           int64_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_uint64_t_argument(size_t   index,
                                                            uint64_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_float_argument(size_t index,
                                                         float  value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_double_argument(size_t index,
                                                          double value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    python_invocation_context::store_long_double_argument(size_t      index,
                                                          long double value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    python_invocation_context::store_string_argument(size_t             index,
                                                     const std::string& value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void python_invocation_context::store_object_argument(
        size_t index, void* value, const mge::script::type_data_ref& t)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::script::invocation_context::call_result_type
    python_invocation_context::call_method(const char* method)
    {
        MGE_DEBUG_TRACE(PYTHON) << "Invoke method: " << method;

        if (m_result) {
            Py_DECREF(m_result);
            m_result = nullptr;
        }

        // Get the method object from the class
        PyObject* method_obj = PyObject_GetAttrString(m_self, method);
        if (!method_obj) {
            return call_result_type::CALL_FAILED;
        }
        if (PyCFunction_Check(method_obj)) {
            PyCFunctionObject* cfunc = (PyCFunctionObject*)method_obj;

            void* cfunc_ptr = cfunc->m_ml->ml_meth;
            void* dispatch_ptr = m_type.method_function(method);

            if (cfunc_ptr == dispatch_ptr) {
                Py_DECREF(method_obj);
                return call_result_type::CALL_FAILED;
            }
        }

        // Create argument tuple with self as first argument
        PyObject* args = PyTuple_New(1);
        if (!args) {
            Py_DECREF(method_obj);
            return call_result_type::CALL_FAILED;
        }
        Py_INCREF(m_self); // Add reference for the tuple
        PyTuple_SetItem(args, 0, m_self);

        // Call the method with self as first argument
        m_result = PyObject_CallObject(method_obj, args);
        Py_DECREF(args);
        Py_DECREF(method_obj);

        if (!m_result) {
            return call_result_type::CALL_FAILED;
        }

        return call_result_type::CALL_EXECUTED;
    }

    bool python_invocation_context::get_bool_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    int8_t python_invocation_context::get_int8_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    uint8_t python_invocation_context::get_uint8_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    int16_t python_invocation_context::get_int16_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    uint16_t python_invocation_context::get_uint16_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    int32_t python_invocation_context::get_int32_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    uint32_t python_invocation_context::get_uint32_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    int64_t python_invocation_context::get_int64_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    uint64_t python_invocation_context::get_uint64_t_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    float python_invocation_context::get_float_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    double python_invocation_context::get_double_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    long double python_invocation_context::get_long_double_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    std::string python_invocation_context::get_string_result()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

} // namespace mge::python
