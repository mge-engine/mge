// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "python.hpp"

namespace mge::python {

    class python_object
    {
    public:
        python_object() noexcept
            : m_ptr(nullptr)
        {}

        explicit python_object(PyObject* ptr) noexcept
            : m_ptr(ptr)
        {}

        ~python_object() { Py_CLEAR(m_ptr); }

        python_object(const python_object& other) noexcept
            : m_ptr(other.m_ptr)
        {
            Py_XINCREF(m_ptr);
        }

        python_object(python_object&& other) noexcept
            : m_ptr(other.m_ptr)
        {
            other.m_ptr = nullptr;
        }

        PyObject* get() const noexcept
        {
            Py_XINCREF(m_ptr);
            return m_ptr;
        }

        PyObject* borrow() const noexcept
        {
            return m_ptr;
        }

        python_object& operator=(const python_object& other) noexcept
        {
            if (this != &other) {
                PyObject* old = m_ptr;
                m_ptr = other.m_ptr;
                Py_XINCREF(m_ptr);
                Py_CLEAR(old);
            }
            return *this;
        }

        python_object& operator=(python_object&& other) noexcept
        {
            if (this != &other) {
                PyObject* old = m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
                Py_CLEAR(old);
            }
            return *this;
        }

    private:
        mutable PyObject* m_ptr;
    };

} // namespace mge::python