// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/is_shared_ptr.hpp"
#include "mge/core/type_name.hpp"
#include <iostream>
#include <memory>
#include <type_traits>
namespace mge {

    /**
     * @brief Helper type for details output.
     *
     * @tparam T contained type
     */
    template <typename T> struct details_type
    {
        explicit details_type(const T* value_)
            : value(value_)
        {}

        details_type(const details_type<T>&) = default;
        details_type(details_type<T>&&) = default;

        details_type<T>& operator=(const details_type<T>&) = default;
        details_type<T>& operator=(details_type<T>&&) = default;

        /**
         * @brief Access to the value to be printed.
         */
        const T* value;
    };

    template <typename T> details_type<T> details(const T& t)
    {
        return details_type<T>(&t);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const details_type<T>& g)
    {
        if constexpr (mge::is_shared_ptr_v<T> || std::is_pointer_v<T>) {
            if (g.value) {
                os << mge::type_name<T>() << " -> " << details(*g.value);
            } else {
                os << mge::type_name<T>() << " -> nullptr";
            }
        } else if constexpr (std::is_trivial_v<T> && !std::is_class_v<T>) {
            os << *g.value;
        } else {
            if constexpr (requires { g.value->details(os); }) {
                if (g.value) {
                    g.value->details(os);
                } else {
                    os << mge::type_name<T>() << " -> nullptr";
                }
            } else {
                os << mge::type_name<T>() << "@" << (void*)g.value;
            }
        }
        return os;
    }

/**
 * @brief Helper macro for details implementation.
 *
 * Provides the prototype for an out of class gist
 * print helper operator. The parameters are
 *
 * - @c os output stream
 * - @c d details helper of type @c TYPE
 *
 * @param TYPE type the details output is implemented for
 */
#define MGE_DETAILS_OUTPUT(TYPE)                                               \
    template <>                                                                \
    inline std::ostream& operator<<(std::ostream&             os,              \
                                    const details_type<TYPE>& g)
} // namespace mge