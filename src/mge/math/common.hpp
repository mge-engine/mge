// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <cmath>
#include <algorithm>

#ifdef min
#  undef min
#endif
#ifdef max
#  undef max
#endif

namespace mge {

    /**
     * @brief Absolute value.
     * @param val argument
     * @return absolute value of the argument
     */
    template <typename T>
    T abs(T val)
    {
        return std::abs(val);
    }

    /**
     * @brief Get minimum of two values.
     * @param a argument
     * @param b argument
     * @return Minimum of @c a and @c b.
     */
    template <typename T>
    T min(T a, T b)
    {
        return std::min(a, b);
    }

    /**
     * @brief Get maximum of two values.
     * @param a argument
     * @param b argument
     * @return Maximum of @c a and @c b.
     */
    template <typename T>
    T max(T a, T b)
    {
        return std::max(a, b);
    }

    /**
     * @brief Square root.
     * @param a argument
     * @return square root of @c a
     */
    template <typename T>
    inline auto sqrt(T a)
    {
        return std::sqrt(a);
    }

    /**
     * Helper structure to define the type of
     * the square root.
     */
    template <typename T>
    struct sqrt_type
    {
        /**
         * @brief type of square root
         */
        using type = decltype(std::sqrt(T()));
    };

}
