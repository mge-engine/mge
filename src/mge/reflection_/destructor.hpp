// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include <functional>
namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT destructor
        {
        public:
            typedef std::function<void (void *)> function;

            destructor() = default;

            destructor(const function& f)
                :m_function(f)
            {}

            destructor& operator =(const destructor& d)
            {
                m_function = d.m_function;
                return *this;
            }

            void invoke(void *);
        private:
            function m_function;
        };
    }
}
