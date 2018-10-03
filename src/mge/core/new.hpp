// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <new>
#include <cstdint>

namespace mge {
    MGE_CORE_EXPORT void *operator_new(std::size_t count);
    MGE_CORE_EXPORT void *operator_new_array(std::size_t count);
    MGE_CORE_EXPORT void *operator_new_noexcept(std::size_t count) noexcept;
    MGE_CORE_EXPORT void *operator_new_array_noexcept(std::size_t count) noexcept;

    MGE_CORE_EXPORT void operator_delete(void *ptr);
    MGE_CORE_EXPORT void operator_delete_array(void *ptr);
    MGE_CORE_EXPORT void operator_delete_noexcept(void *ptr) noexcept;
    MGE_CORE_EXPORT void operator_delete_array_noexcept(void *ptr) noexcept;

    MGE_CORE_EXPORT uint64_t allocation_count();
    MGE_CORE_EXPORT uint64_t thread_allocation_count();

    class MGE_CORE_EXPORT allocation_count_scope
    {
    public:
        inline allocation_count_scope()
            :m_count(thread_allocation_count())
        {}

        inline ~allocation_count_scope() = default;

        uint64_t allocations_in_scope() const
        {
            return thread_allocation_count() - m_count;
        }
    private:
        uint64_t m_count;
    };
}

inline void* operator new ( std::size_t count )
{
    return mge::operator_new(count);
}

inline void* operator new[]( std::size_t count )
{
    return mge::operator_new_array(count);
}

inline void* operator new  ( std::size_t count, const std::nothrow_t& tag)
{
    return mge::operator_new_noexcept(count);
}

inline void* operator new[]( std::size_t count, const std::nothrow_t& tag)
{
    return mge::operator_new_array_noexcept(count);
}

inline void operator delete  ( void* ptr )
{
    mge::operator_delete(ptr);
}

inline void operator delete[]( void* ptr )
{
    mge::operator_delete_array(ptr);
}

inline void operator delete  ( void* ptr, const std::nothrow_t& tag )
{
    mge::operator_delete_noexcept(ptr);
}

inline void operator delete[]( void* ptr, const std::nothrow_t& tag )
{
    mge::operator_delete_array_noexcept(ptr);
}

