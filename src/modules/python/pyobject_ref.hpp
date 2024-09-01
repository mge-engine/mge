#pragma once
#include "python.hpp"

namespace mge::python {

    class pyobject_ref
    {
    public:
        pyobject_ref() {}

        pyobject_ref(PyObject* object)
            : m_object(object)
        {}

        pyobject_ref(const pyobject_ref& other)
            : m_object(other.m_object)
        {
            Py_XINCREF(m_object);
        }

        pyobject_ref(pyobject_ref&& other)
            : m_object(other.m_object)
        {
            other.m_object = nullptr;
        }

        pyobject_ref& operator=(const pyobject_ref& other)
        {
            if (this != &other && m_object != other.m_object) {
                auto object = m_object;
                m_object = other.m_object;
                Py_XINCREF(m_object);
                Py_CLEAR(object);
            }
            return *this;
        }

        pyobject_ref& operator=(pyobject_ref&& other)
        {
            if (this != &other && m_object != other.m_object) {
                auto object = m_object;
                m_object = other.m_object;
                other.m_object = nullptr;
                Py_CLEAR(object);
            }
            return *this;
        }

        ~pyobject_ref() { Py_CLEAR(m_object); }

        PyObject* get() const noexcept { return m_object; }

        PyObject*       operator->() noexcept { return m_object; }
        const PyObject* operator->() const noexcept { return m_object; }

        inline operator bool() const noexcept { return m_object != nullptr; }

    private:
        PyObject* m_object{nullptr};
    };

} // namespace mge::python