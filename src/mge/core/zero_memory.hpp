#pragma once
#include <memory>
namespace mge {

    template <typename T>
    inline void zero_memory(T& value)
    {
        memset(std::addressof(value), 0, sizeof(T));
    }

    template <typename T>
    inline void zero_memory(T* value)
    {
        memset(value, 0, sizeof(T));
    }

}
