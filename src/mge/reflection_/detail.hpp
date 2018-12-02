#pragma once
#include "mge/reflection/dllexport.hpp"

#define MGE_ENABLE_MEMBER_IF(cond)                      \
    template <typename X=void,                          \
              typename = std::enable_if<cond, X>::type>

namespace mge {
    namespace reflection {
        namespace detail {
            template <typename T>
            inline size_t size_of_type()
            {
                return sizeof(T);
            }

            template <>
            inline size_t size_of_type<void>()
            {
                return 0;
            }
        }
    }
}
