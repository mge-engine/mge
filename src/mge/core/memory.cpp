// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    void* ::mge::malloc(size_t size)
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

    void* realloc(void* ptr, size_t size)
    {
        void* new_ptr = ::realloc(ptr, size);
        if (new_ptr == nullptr && size > 0) {
            MGE_THROW(out_of_memory)
                << "Cannot reallocate " << size << " bytes";
        }
        return new_ptr;
    }

} // namespace mge