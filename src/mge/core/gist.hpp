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

    namespace {
        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void print_value(std::ostream& os, const T& value)
        {
            os << value;
        }

        template <typename T, std::enable_if_t<std::is_class_v<T>, bool> = true>
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