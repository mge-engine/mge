// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/type_name.hpp"
#include "mge/core/type_traits.hpp"
#include "mge/core/types.hpp"
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

        /**
         * @brief Access to the value to be printed.
         */
        const T* value;
    };

    template <typename T> details_type<T> gist(const T& t)
    {
        return details_type<T>(&t);
    }

    template <typename T> struct has_details_method
    {
        /* SFINAE correct signature */
        template <typename A>
        static std::true_type test(void (A::*)(std::ostream&) const)
        {
            return std::true_type();
        }

        /* SFINAE operator-exists */
        template <typename A>
        static decltype(test(&A::details)) test(decltype(&A::details), int)
        {
            using return_type = decltype(test(&A::gist));
            return return_type();
        }
        /* SFINAE don't have it*/
        template <typename A> static std::false_type test(...)
        {
            return std::false_type();
        }

        /* This will be either `std::true_type` or `std::false_type` */
        using type = decltype(test<T>(0, 0));
        static constexpr bool value = type::value; /* Which is it? */
    };

    namespace {
        template <typename T,
                  std::enable_if_t<has_details_method<T>::value, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            value.details(os);
        }

        template <typename T,
                  std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            os << value;
        }

        template <typename T,
                  std::enable_if_t<std::is_class_v<T> &&
                                       !mge::is_shared_ptr<T>::value &&
                                       !has_details_method<T>::value,
                                   bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            os << mge::type_name<T>() << "@" << (void*)&value;
        }

        template <typename T,
                  std::enable_if_t<std::is_class_v<T> &&
                                       mge::is_shared_ptr<T>::value &&
                                       !has_gist_method<T>::value,
                                   bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            if (value) {
                os << mge::type_name<T>() << " => ";
                print_value(os, *value);
            } else {
                os << mge::type_name<T>() << " => nullptr";
            }
        }

    } // namespace

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const details_type<T>& g)
    {
        print_value(os, *g.value);
        return os;
    }

/**
 * @brief Helper macro for gist implementation.
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