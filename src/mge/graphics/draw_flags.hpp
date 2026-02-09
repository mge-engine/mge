// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"

#include <functional>

namespace mge {

    /**
     * @brief Draw flags used to control rendering state.
     *
     * These flags define properties of the drawing pipeline, e.g.
     * whether to enable depth testing, blending, face culling, etc.
     */
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
        static constexpr uint64_t FRONT_FACE_COUNTER_CLOCKWISE = 1ull << 9;
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
        static constexpr uint64_t DEPTH_BIAS_ENABLE = 1ull << 17;
        static constexpr uint64_t DEPTH_CLAMP_ENABLE = 1ull << 18;
        static constexpr uint64_t LAST = DEPTH_CLAMP_ENABLE;
        static constexpr uint64_t ALL_FLAGS = (LAST << 1) - 1;

        static constexpr uint64_t COLOR_BLEND_FACTOR_SRC_SHIFT = 19;
        static constexpr uint64_t COLOR_BLEND_FACTOR_SRC_MASK =
            0x1Full << COLOR_BLEND_FACTOR_SRC_SHIFT;
        static constexpr uint64_t COLOR_BLEND_FACTOR_DST_SHIFT = 24;
        static constexpr uint64_t COLOR_BLEND_FACTOR_DST_MASK =
            0x1Full << COLOR_BLEND_FACTOR_DST_SHIFT;
        static constexpr uint64_t COLOR_BLEND_OPERATION_SHIFT = 29;
        static constexpr uint64_t COLOR_BLEND_OPERATION_MASK =
            0x7ull << COLOR_BLEND_OPERATION_SHIFT;

        static constexpr uint64_t ALPHA_BLEND_FACTOR_SRC_SHIFT = 32;
        static constexpr uint64_t ALPHA_BLEND_FACTOR_SRC_MASK =
            0x1Full << ALPHA_BLEND_FACTOR_SRC_SHIFT;
        static constexpr uint64_t ALPHA_BLEND_FACTOR_DST_SHIFT = 37;
        static constexpr uint64_t ALPHA_BLEND_FACTOR_DST_MASK =
            0x1Full << ALPHA_BLEND_FACTOR_DST_SHIFT;
        static constexpr uint64_t ALPHA_BLEND_OPERATION_SHIFT = 42;
        static constexpr uint64_t ALPHA_BLEND_OPERATION_MASK =
            0x7ull << ALPHA_BLEND_OPERATION_SHIFT;

        static constexpr uint64_t PRIMITIVE_TOPOLOGY_SHIFT = 45;
        static constexpr uint64_t PRIMITIVE_TOPOLOGY_MASK =
            0x3ull << PRIMITIVE_TOPOLOGY_SHIFT;
        static constexpr uint64_t PRIMITIVE_TOPOLOGY_TRIANGLE_LIST =
            0ull << PRIMITIVE_TOPOLOGY_SHIFT;
        static constexpr uint64_t PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP =
            1ull << PRIMITIVE_TOPOLOGY_SHIFT;
        static constexpr uint64_t PRIMITIVE_TOPOLOGY_LINE_LIST =
            2ull << PRIMITIVE_TOPOLOGY_SHIFT;
        static constexpr uint64_t PRIMITIVE_TOPOLOGY_POINT_LIST =
            3ull << PRIMITIVE_TOPOLOGY_SHIFT;

        static constexpr uint64_t MSAA_SAMPLE_COUNT_SHIFT = 47;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_MASK =
            0x7ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_1 =
            0ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_2 =
            1ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_4 =
            2ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_8 =
            3ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_16 =
            4ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_32 =
            5ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_SAMPLE_COUNT_64 =
            6ull << MSAA_SAMPLE_COUNT_SHIFT;
        static constexpr uint64_t MSAA_ALPHA_TO_COVERAGE = 1ull << 50;
        static constexpr uint64_t MSAA_SAMPLE_SHADING = 1ull << 51;
        static constexpr uint64_t CONSERVATIVE_RASTERIZATION = 1ull << 52;

        static constexpr uint64_t ALL_VALID_BITS =
            ALL_FLAGS | COLOR_BLEND_FACTOR_SRC_MASK |
            COLOR_BLEND_FACTOR_DST_MASK | COLOR_BLEND_OPERATION_MASK |
            ALPHA_BLEND_FACTOR_SRC_MASK | ALPHA_BLEND_FACTOR_DST_MASK |
            ALPHA_BLEND_OPERATION_MASK | PRIMITIVE_TOPOLOGY_MASK |
            MSAA_SAMPLE_COUNT_MASK | MSAA_ALPHA_TO_COVERAGE |
            MSAA_SAMPLE_SHADING | CONSERVATIVE_RASTERIZATION;

        static constexpr uint64_t DEFAULT = DEPTH_WRITE | COLOR_WRITE |
                                            PRIMITIVE_TOPOLOGY_TRIANGLE_LIST |
                                            MSAA_SAMPLE_COUNT_1;

        draw_flags() = default;

        draw_flags(uint64_t raw_flags)
            : m_flags(raw_flags)
        {
            if ((raw_flags & ~ALL_VALID_BITS) != 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid draw flags: " << raw_flags;
            }
        }

        draw_flags(const std::initializer_list<uint64_t>& flags)
            : m_flags(0)
        {
            for (auto flag : flags) {
                if ((flag & ~ALL_VALID_BITS) != 0) {
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
            if ((flag & ~ALL_VALID_BITS) != 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Invalid draw flag: " << flag;
            }
            m_flags |= flag;
        }

        void clear(uint64_t flag)
        {
            if ((flag & ~ALL_VALID_BITS) != 0) {
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