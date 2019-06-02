#pragma once
namespace mge {
    template <typename T>
    inline void *to_void_ptr(T *ptr)
    {
        return static_cast<void *>(ptr);
    }

    template <typename T>
    inline void *to_void_ptr(const T *ptr)
    {
        return const_cast<void *>(static_cast<const void *>(ptr));
    }
}

