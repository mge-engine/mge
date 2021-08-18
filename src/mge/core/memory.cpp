// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    void* mge::malloc(size_t size)
    {
        void* ptr = ::malloc(size);
        if (ptr == nullptr) {
            MGE_THROW(out_of_memory) << "Cannot allocate " << size << " bytes";
        }
        return ptr;
    }

    void free(void* ptr)
    {
        if (ptr == nullptr) {
            return;
        }
        ::free(ptr);
    }

} // namespace mge