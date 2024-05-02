// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/signature.hpp"

namespace mge::script {
    signature::signature() {}

    signature::~signature() {}

    bool signature::empty() const { return m_types.empty(); }

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

    bool signature::matches(const signature& other) const
    {
        return *this == other;
    }

    static signature s_empty_signature;

    const signature& signature::empty_signature() { return s_empty_signature; }

    void signature::gist(std::format_context& ctx) const
    {
        std::format_to(ctx.out(), "(");
        auto it = m_types.begin();
        if (it != m_types.end()) {
            if (it->has_value()) {
                std::format_to(ctx.out(), "{}", it->value().name());
            } else {
                std::format_to(ctx.out(), "?");
            }
            ++it;
            while (it != m_types.end()) {
                if (it->has_value()) {
                    std::format_to(ctx.out(), ", {}", it->value().name());
                } else {
                    std::format_to(ctx.out(), ", ?");
                }
                ++it;
            }
        }
        std::format_to(ctx.out(), ")");
    }

} // namespace mge::script