// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/vertex_layout.hpp"
#include "mge/core/format.hpp"
#include <iostream>
#include <numeric>
#include <ranges>

namespace mge {

    vertex_layout::vertex_layout() {}

    vertex_layout::vertex_layout(const std::initializer_list<vertex_format>& l)
    {
        m_formats.reserve(l.size());
        m_semantics.reserve(l.size());
        for (const auto f : l) {
            m_formats.push_back(f);
            m_semantics.push_back(mge::attribute_semantic::ANY);
        }
    }

    size_t vertex_layout::binary_size() const
    {
        auto r = m_formats | std::views::transform([](const auto& vf) {
                     return vf.binary_size();
                 }) |
                 std::views::common;
        const size_t zero = 0;
        return std::accumulate(std::ranges::begin(r),
                               std::ranges::end(r),
                               zero);
    }

    void vertex_layout::push_back(const vertex_format& f)
    {
        m_formats.push_back(f);
        m_semantics.push_back(mge::attribute_semantic::ANY);
    }

    void vertex_layout::format(std::format_context& context) const
    {
        std::format_to(context.out(), "[");
#if 0        
        auto it = m_formats.cbegin();
        auto sem_it = m_semantics.cbegin();
        std::format_to(context.out(),
                       "{{format={}, semantic={}}}",
                       *it,
                       *sem_it);
        ++it;
        ++sem_it;
        while (it != m_formats.cend()) {
            std::format_to(context.out(),
                           ", {{format={}, semantic={}}}",
                           *it,
                           *sem_it);
            ++it;
            ++sem_it;
        }
#endif
        format_to(context.out(), "]");
    }

    vertex_layout parse_vertex_layout(std::string_view sv)
    {
        if (sv.size() < 2) {
            MGE_THROW(illegal_argument) << "Invalid vertex layout: " << sv;
        }

        auto it = sv.cbegin();
        if (*it != '[') {
            MGE_THROW(illegal_argument) << "Invalid vertex layout: " << sv;
        }
        ++it;
        if (*sv.rbegin() != ']') {
            MGE_THROW(illegal_argument) << "Invalid vertex layout: " << sv;
        }
        auto e = sv.cend();
        --e;
        vertex_layout result;
        while (it != e) {
            auto next_comma = it;
            while (next_comma != e && *next_comma != ',') {
                ++next_comma;
            }
            std::string_view next_format(it, next_comma);
            result.push_back(parse_vertex_format(next_format));
            it = next_comma;
            ++it;
            while (it != e && std::isspace(*it)) {
                ++it;
            }
        }
        return result;
    }

} // namespace mge