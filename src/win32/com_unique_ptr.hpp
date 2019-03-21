#pragma once
#include <memory>

namespace mge {

    template <typename T>
    struct com_deleter
    {
        typedef T *pointer;
        com_deleter() = default;
        inline void operator()(T *ptr) const { ptr->Release(); }
    };
}

#define COM_UNIQUE_PTR(T) std::unique_ptr<T, mge::com_deleter<T>>


