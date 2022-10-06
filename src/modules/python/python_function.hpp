// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"

#include "python.hpp"
#include "python_fwd.hpp"
#include "python_object.hpp"

namespace mge::python {

    class python_function : public std::enable_shared_from_this<python_function>
    {
    public:
        python_function(const std::string&                  name,
                        const std::type_index&              return_type,
                        const mge::script::signature&       sig,
                        const mge::script::invoke_function& invoke);
        ~python_function();

        const std::string& name() const { return m_name; }
        PyObject*          py_object() const;

        static void init(PyObject* module);

        void interpreter_lost();
        void add_signature(const std::type_index&              return_type,
                           const mge::script::signature&       sig,
                           const mge::script::invoke_function& invoke);

    private:
        const std::string& m_name;

        struct details
        {
            const std::type_index*              return_type;
            const mge::script::signature*       signature;
            const mge::script::invoke_function* invoke;
        };

        mge::small_vector<details, 1> m_details;
        mutable python_object         m_object;

        static PyObject* call(PyObject* self, PyObject* args, PyObject* kwargs);
        static void      dealloc(PyObject* self);

        // clang-format off
        struct python_function_pyobject
        {
            PyObject_HEAD
            python_function_ref method;
        };
        // clang-format on

        static inline python_function_pyobject* to_function_object(PyObject* o)
        {
            return reinterpret_cast<python_function_pyobject*>(o);
        }

        static PyTypeObject s_type;
    };

} // namespace mge::python
