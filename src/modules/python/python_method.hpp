#pragma once
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"

#include "python.hpp"
#include "python_fwd.hpp"
#include "python_object.hpp"

namespace mge::python {

    class python_method : public std::enable_shared_from_this<python_method>
    {
    public:
        python_method(const python_type_ref&              type,
                      const std::string&                  name,
                      const std::type_index&              return_type,
                      const mge::script::signature&       sig,
                      const mge::script::invoke_function& invoke);
        ~python_method();

        const std::string& name() const { return m_name; }
        PyObject*          py_object() const;

        static void init(PyObject* module);

        void interpreter_lost();

    private:
        python_type_ref                     m_type;
        const std::string&                  m_name;
        const std::type_index&              m_return_type;
        const mge::script::signature&       m_signature;
        const mge::script::invoke_function& m_invoke;
        mutable python_object               m_object;

        static PyObject* call(PyObject* self, PyObject* args, PyObject* kwargs);
        static void      dealloc(PyObject* self);

        // clang-format off
        struct python_method_pyobject
        {
            PyObject_HEAD
            python_method_ref method;
        };
        // clang-format on
        static inline python_method_pyobject* to_method_object(PyObject* o)
        {
            return reinterpret_cast<python_method_pyobject*>(o);
        }

        static PyTypeObject s_type;
    };

} // namespace mge::python
