// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"

#include <functional>

namespace mge {

    class MGEGRAPHICS_EXPORT draw_flags
    {
    public:
        static constexpr uint64_t NONE = 0;
        static constexpr uint64_t DEPTH_TEST_LESS = 1ull << 0;
        static constexpr uint64_t DEPTH_TEST_LESS_EQUAL = 1ull << 1;
        static constexpr uint64_t DEPTH_TEST_EQUAL = 1ull << 2;
        static constexpr uint64_t DEPTH_TEST_GREATER = 1ull << 3;
        static constexpr uint64_t DEPTH_TEST_GREATER_EQUAL = 1ull << 4;
        static constexpr uint64_t DEPTH_TEST_NOT_EQUAL = 1ull << 5;
        static constexpr uint64_t DEPTH_TEST_ALWAYS = 1ull << 6;
        static constexpr uint64_t DEPTH_WRITE = 1ull << 7;
        static constexpr uint64_t STENCIL_TEST = 1ull << 8;
        static constexpr uint64_t BLEND = 1ull << 9;
        static constexpr uint64_t STENCIL_WRITE = 1ull << 10;
        static constexpr uint64_t RED_WRITE = 1ull << 11;
        static constexpr uint64_t GREEN_WRITE = 1ull << 12;
        static constexpr uint64_t BLUE_WRITE = 1ull << 13;
        static constexpr uint64_t ALPHA_WRITE = 1ull << 14;
        static constexpr uint64_t COLOR_WRITE =
            RED_WRITE | GREEN_WRITE | BLUE_WRITE | ALPHA_WRITE;
        static constexpr uint64_t RGB_WRITE =
            RED_WRITE | GREEN_WRITE | BLUE_WRITE;
        static constexpr uint64_t RGBA_WRITE =
            RED_WRITE | GREEN_WRITE | BLUE_WRITE | ALPHA_WRITE;
        static constexpr uint64_t CULL_CLOCK_WISE = 1ull << 15;
        static constexpr uint64_t CULL_COUNTER_CLOCK_WISE = 1ull << 16;
        static constexpr uint64_t LAST = CULL_COUNTER_CLOCK_WISE;
        static constexpr uint64_t ALL_FLAGS = (LAST << 1) - 1;

        draw_flags() = default;

        draw_flags(uint64_t raw_flags)
            : m_flags(raw_flags)
        {
            if ((raw_flags & ~ALL_FLAGS) != 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid draw flags: " << raw_flags;
            }
        }

        draw_flags(const std::initializer_list<uint64_t>& flags)
            : m_flags(0)
        {
            for (auto flag : flags) {
                if ((flag & ~ALL_FLAGS) != 0) {
                    MGE_THROW(mge::illegal_argument)
                        << "Invalid draw flag: " << flag;
                }
                m_flags |= flag;
            }
        }

        draw_flags(const draw_flags&) = default;
        draw_flags(draw_flags&&) noexcept = default;
        draw_flags& operator=(const draw_flags&) = default;
        draw_flags& operator=(draw_flags&&) noexcept = default;

        uint64_t raw() const noexcept
        {
            return m_flags;
        }

        void set(uint64_t flag)
        {
            if ((flag & ~ALL_FLAGS) != 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid draw flag: " << flag;
            }
            m_flags |= flag;
        }

        void clear(uint64_t flag)
        {
            if ((flag & ~ALL_FLAGS) != 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid draw flag: " << flag;
            }
            m_flags &= ~flag;
        }

        bool test(uint64_t flag) const noexcept
        {
            return (m_flags & flag) != 0;
        }

        bool empty() const noexcept
        {
            return m_flags == 0;
        }

        void clear() noexcept
        {
            m_flags = 0;
        }

        bool includes(const draw_flags& other) const noexcept
        {
            return (m_flags & other.m_flags) == other.m_flags;
        }

    private:
        uint64_t m_flags{0};
    };

} // namespace mge

template <>
struct fmt::formatter<mge::draw_flags> : public fmt::formatter<uint64_t>
{
    template <typename FormatContext>
    auto format(const mge::draw_flags& flags, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "draw_flags{{{:#018x}}}", flags.raw());
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const draw_flags& flags)
    {
        fmt::print(os, "{}", flags);
        return os;
    }
} // namespace mge

namespace std {
    template <> struct hash<mge::draw_flags>
    {
        size_t operator()(const mge::draw_flags& flags) const noexcept
        {
            return std::hash<uint64_t>{}(flags.raw());
        }
    };
} // namespace std