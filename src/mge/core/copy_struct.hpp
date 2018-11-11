#pragma once
#include <cstdlib>
#include <memory>
namespace mge {

    template <typename T>
    inline void copy_struct(T& target, const T& source)
    {
        memcpy(std::addressof(target),
               std::addressof(source),
               sizeof(T));
    }

    template <typename T>
    inline void copy_struct(T* target, const T& source)
    {
        memcpy(target,
               std::addressof(source),
               sizeof(T));
    }

}
