#pragma once
#include "mge/reflection/dllexport.hpp"
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
