// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/core/log.hpp"

MGE_DEFINE_LOG(MEMORY);

namespace mge {



    void *malloc(size_t sz)
    {
        return ::malloc(sz);
    }

    void free(void *ptr)
    {
        ::free(ptr);
    }

    void *realloc(void *ptr, size_t newsz)
    {
        return ::realloc(ptr, newsz);
    }

    void *calloc(size_t num, size_t size)
    {
        return ::calloc(num, size);
    }

}
