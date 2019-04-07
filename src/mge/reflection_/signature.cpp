// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/signature.hpp"
namespace mge {
    namespace reflection {
        signature::signature(const signature& s)
            :m_types(s.m_types)
        {}

        signature::signature(signature&& s)
            :m_types(std::move(s.m_types))
        {}

        signature::signature(const std::initializer_list<std::type_index>& elements)
        {
            m_types.assign(elements.begin(), elements.end());
        }

        signature&
        signature::operator =(const signature& s)
        {
            m_types = s.m_types;
            return *this;
        }

        signature&
        signature::operator =(signature&& s)
        {
            m_types = std::move(s.m_types);
            return *this;
        }

        std::ostream&
        operator <<(std::ostream& os, const signature& s)
        {
            os << "(";
            bool first = true;
            for(auto ti : s) {
                if(!first) {
                    os << ", ";
                }
                first=false;
                os << ti.name();
            }
            os << ")";
            return os;
        }

    }
}
