// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

#define JEMALLOC_NO_RENAME
#include <jemalloc/jemalloc.h>

#define MALLOC je_malloc
#define REALLOC je_realloc
#define FREE je_free
namespace mge {

#if MGE_COMPILER_MSVC
    void* ::mge::malloc(size_t size)
#else
    void* malloc(size_t size)
#endif
    {
        void* ptr = ::MALLOC(size);
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
        ::FREE(ptr);
    }

    void* realloc(void* ptr, size_t size)
    {
        void* new_ptr = ::REALLOC(ptr, size);
        if (new_ptr == nullptr && size > 0) {
            MGE_THROW(out_of_memory)
                << "Cannot reallocate " << size << " bytes";
        }
        return new_ptr;
    }

    MGECORE_EXPORT void* allocate(size_t bytes, size_t alignment)
    {
        void* ptr = je_aligned_alloc(alignment, bytes);
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
        ::je_free(ptr);
    }

} // namespace mge