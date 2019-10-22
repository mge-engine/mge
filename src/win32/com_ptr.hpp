// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>
#include <windows.h>

namespace mge {


    template <typename T>
    class com_ptr
    {
    public:
        typedef T element_type;

        com_ptr()
            :m_element(nullptr)
        {}

        template <typename U>
        com_ptr(U *ptr)
            :m_element(ptr)
        {
            maybe_add_ref();
        }

        template <typename U>
        com_ptr(const com_ptr<U>& other)
            :m_element(other.m_element)
        {
            maybe_add_ref();
        }

        com_ptr(com_ptr<T>&& other)
        {
            swap(std::move(other));
        }

        ~com_ptr()
        {
            maybe_release();
        }

        com_ptr<T>& operator=(T* other)
        {
            if (m_element != other) {
                com_ptr<T>(other).swap(*this);
            }
            return *this;
        }

        template<typename U>
        com_ptr<T>& operator=(const U* other)
        {
            com_ptr<T>(other).swap(*this);
            return *this;
        }

        com_ptr<T>& operator =(const com_ptr<T>& other)
        {
            if (m_element != other.m_element) {
                com_ptr<T>(other).swap(*this);
            }
            return *this;
        }

        template <typename U>
        com_ptr<T>& operator =(const com_ptr<U>& other)
        {
            com_ptr<T>(other).swap(*this);
            return *this;
        }

        void swap(com_ptr<T>&& other)
        {
            std::swap(m_element, other.m_element);
        }

        void swap(com_ptr<T>& other)
        {
            std::swap(m_element, other.m_element);
        }

        void reset()
        {
            maybe_release();
            m_element = nullptr;
        }

        template<typename U>
        void reset(U *ptr)
        {
            com_ptr<T>(ptr).swap(*this);
        }

        operator bool () const noexcept { return m_element != nullptr; }
        T* get() const noexcept { return m_element; }
        T* operator ->() const noexcept { return m_element; }
        T& operator *() const noexcept { return *m_element; }

    private:
        IUnknown *element() { return m_element; }

        void maybe_add_ref()
        {
            if (m_element) {
                element()->AddRef();
            }
        }

        void maybe_release()
        {
            if (m_element) {
                element()->Release();
            }
        }

        T* m_element;
    };

    template <typename T1, typename T2>
    inline bool operator ==(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() == b.get();
    }

    template <typename T1, typename T2>
    inline bool operator !=(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() != b.get();
    }

    template <typename T1, typename T2>
    inline bool operator <(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() < b.get();
    }

    template <typename T1, typename T2>
    inline bool operator <=(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() <= b.get();
    }

    template <typename T1, typename T2>
    inline bool operator >(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() > b.get();
    }

    template <typename T1, typename T2>
    inline bool operator >=(const com_ptr<T1>& a, const com_ptr<T2>& b)
    {
        return a.get() >= b.get();
    }

}
