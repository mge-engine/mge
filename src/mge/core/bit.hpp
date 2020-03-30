// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/types.hpp"
#ifdef MGE_COMPILER_MSVC
#    include <intrin.h>
#endif
#include <cstdint>
namespace mge {

    /**
     * @brief Number of bits set in a value.
     *
     * @param x        value to test
     * @return number of bits set
     */
    inline int popcount(uint8_t x)
    {
#ifdef MGE_COMPILER_MSVC
        return static_cast<int>(__popcnt16(static_cast<uint16_t>(x)));
#else
        x = x - ((x >> 1) & 0x55);
        x = (x & 0x33) + ((x >> 2) & 0x33);
        return static_cast<int>((x + (x >> 4)) & 0x0F);
#endif
    }

    /**
     * @brief Number of bits set in a value.
     *
     * @param x        value to test
     * @return number of bits set
     */
    inline int popcount(uint16_t x)
    {
#ifdef MGE_COMPILER_MSVC
        return static_cast<int>(__popcnt16(x));
#else
        x = x - ((x >> 1) & 0x5555);
        x = (x & 0x3333) + ((x >> 2) & 0x3333);
        return static_cast<int>(((x + (x >> 4)) & 0x0F0F) * 0x0101) >> 8;
#endif
    }

    /**
     * @brief Number of bits set in a value.
     *
     * @param x        value to test
     * @return number of bits set
     */
    inline int popcount(uint32_t x)
    {
#ifdef MGE_COMPILER_MSVC
        return static_cast<int>(__popcnt(x));
#else
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        return static_cast<int>((((x + (x >> 4)) & 0x0F0F0F0F) * 0x01010101) >>
                                24);
#endif
    }

    /**
     * @brief Number of bits set in a value.
     *
     * @param x        value to test
     * @return number of bits set
     */
    inline int popcount(uint64_t x)
    {
#ifdef MGE_COMPILER_MSVC
        return static_cast<int>(__popcnt64(x));
#else
        x = x - ((x >> 1) & 0x5555555555555555);
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
        return static_cast<int>(
            (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 56);
#endif
    }

} // namespace mge