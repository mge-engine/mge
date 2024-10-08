#pragma once
#include "mge/script/script_fwd.hpp"
#include "pyobject_ref.hpp"
#include "python_fwd.hpp"

#include <string>

namespace mge::python {

    class python_function
    {
    public:
        python_function(python_context&                       context,
                        const mge::script::function_data_ref& function);
        virtual ~python_function() = default;
        void on_interpreter_loss();
        void on_interpreter_restore();

        static void register_function_type(const python_module_ref& mod);

    private:
        static void tp_dealloc(PyObject* self);
        static PyObject*
        tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
        static PyObject*
        tp_call(PyObject* self, PyObject* args, PyObject* kwds);

        PyObject* call(PyObject* args, PyObject* kwds);
        void      initialize();

        python_context& m_context;
        std::string     m_name;
        std::string     m_module_name;

        pyobject_ref                   m_object;
        mge::script::function_data_ref m_function;

        struct python_function_object
        {
            // clang-format off
            PyObject_HEAD
            python_function_ref function;
            // clang-format on
        };

        static PyTypeObject s_type;
    };
} // namespace mge::python