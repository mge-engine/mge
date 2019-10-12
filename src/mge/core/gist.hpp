#pragma once
#include "mge/core/types.hpp"
#include <iostream>
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

    template <typename T>
    std::ostream& operator <<(std::ostream& os, const gist_type<T>& g)
    {
        return os << *g.value;
    }

#define GIST_OUTPUT(TYPE)                                                   \
    template<>                                                              \
    inline                                                                  \
    std::ostream& operator <<(std::ostream& os, const gist_type<TYPE>& g)   
}