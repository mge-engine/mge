// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <memory_resource>

namespace mge {

    /**
     * @brief Memory resource.
     * This memory resource uses the fast allocator provided with mge.
     *
     * Note that you should not create instances of this memory resource,
     * but rather use the static instance provided.
     */
    class MGECORE_EXPORT memory_resource : public std::pmr::memory_resource
    {
    protected:
        void* do_allocate(size_t bytes, size_t alignment) override;
        void  do_deallocate(void* p, size_t bytes, size_t alignment) override;
        bool  do_is_equal(
             const std::pmr::memory_resource& other) const noexcept override;

    public:
        /**
         * @brief Instance of the memory resource.
         */
        static ::mge::memory_resource instance;
        /**
         * @brief Allocator using the memory resource.
         */
        static std::pmr::polymorphic_allocator<void> allocator;
    };

} // namespace mge