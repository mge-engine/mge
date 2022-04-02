// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/signature.hpp"

namespace mge::script {
    signature::signature() {}

    signature::~signature() {}

    bool signature::operator==(const signature& s) const
    {
        if (s.m_types.size() == m_types.size()) {
            for (size_t i = 0; i < m_types.size(); ++i) {
                if (m_types[i].has_value() && s.m_types[i].has_value()) {
                    if (*m_types[i] != *s.m_types[i]) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    const std::type_index& signature::operator[](size_t i) const
    {
        return *m_types[i];
    }

    bool signature::matches(const signature& other) const
    {
        return *this == other;
    }

} // namespace mge::script