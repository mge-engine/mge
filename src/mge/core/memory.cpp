// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

#include <cstdlib>
#ifdef MGE_OS_WINDOWS
#    include <malloc.h>
#endif

namespace mge {

    void* malloc(size_t size)
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

    MGECORE_EXPORT void* allocate(size_t bytes, size_t alignment)
    {
#ifdef MGE_OS_WINDOWS
        void* ptr = ::_aligned_malloc(bytes, alignment);
#else
        void* ptr = ::aligned_alloc(alignment, bytes);
#endif
        if (ptr == nullptr) {
            MGE_THROW(out_of_memory) << "Cannot allocate " << bytes
                                     << " bytes with alignment " << alignment;
        }
        return ptr;
    }

    MGECORE_EXPORT void deallocate(void* ptr, size_t bytes, size_t alignment)
    {
        if (ptr == nullptr) {
            return;
        }
#ifdef MGE_OS_WINDOWS
        ::_aligned_free(ptr);
#else
        ::free(ptr);
#endif
    }

} // namespace mge