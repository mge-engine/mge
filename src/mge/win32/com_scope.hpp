// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>

namespace mge {

    template <typename T> struct com_scope
    {
        T* m_ptr;

        com_scope(T* ptr)
            : m_ptr(ptr)
        {}

        ~com_scope()
        {
            if (m_ptr)
                m_ptr->Release();
        }

        void reset()
        {
            m_ptr = nullptr;
        }
    };
} // namespace mge