#pragma once
#include "mge/core/types.hpp"
#include "mge/core/type_name.hpp"
#include <iostream>
#include <type_traits>

namespace mge {

    template <typename T>
    struct gist_type
    {
        explicit gist_type(const T *value_)
            :value(value_)
        {}

        const T* value;
    };

    template <typename T>
    gist_type<T> gist(const T& t)
    {
        return gist_type<T>(&t);
    }

        template< typename T>
        struct has_gist_method
        {
            /* SFINAE correct signature */
            template<typename A>
            static std::true_type test(void (A::*)(std::ostream&) const) {
                return std::true_type();
            }

            /* SFINAE operator-exists */
            template <typename A> 
            static decltype(test(&A::gist)) 
            test(decltype(&A::gist), int) {
                typedef decltype(test(&A::gist)) return_type; 
                return return_type();
            }
            /* SFINAE don't have it*/
            template<typename A>
            static std::false_type test(...) {
                return std::false_type(); 
            }

            /* This will be either `std::true_type` or `std::false_type` */
            typedef decltype(test<T>(0,0)) type;
            static constexpr bool value = type::value; /* Which is it? */
        };


    namespace {
        template <typename T, std::enable_if_t<has_gist_method<T>::value, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            value.gist(os);
        }


        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            os << value;
        }

        template <typename T, std::enable_if_t<std::is_class_v<T> && !has_gist_method<T>::value, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            os << mge::type_name<T>() << "@" << (void *)&value;
        }
    }


    template <typename T>
    std::ostream& operator <<(std::ostream& os, const gist_type<T>& g)
    {
        print_value(os, *g.value);
        return os;
    }

#define GIST_OUTPUT(TYPE)                                                   \
    template<>                                                              \
    inline                                                                  \
    std::ostream& operator <<(std::ostream& os, const gist_type<TYPE>& g)   
}