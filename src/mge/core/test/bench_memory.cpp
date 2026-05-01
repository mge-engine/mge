// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include "test/googletest.hpp"

#include "mge/core/memory.hpp"
#include <memory_resource>

TEST(benchmark, malloc_free)
{
    mge::benchmark()
        .run("alloc_free_64",
             [&]() {
                 auto ptr = mge::malloc(64);
                 mge::free(ptr);
             })
        .run("alloc_free_128",
             [&]() {
                 auto ptr = mge::malloc(128);
                 mge::free(ptr);
             })
        .run("alloc_free_512",
             [&]() {
                 auto ptr = mge::malloc(512);
                 mge::free(ptr);
             })
        .run("alloc_free_1024",
             [&]() {
                 auto ptr = mge::malloc(1024);
                 mge::free(ptr);
             })
        .run("alloc_free_1M",
             [&]() {
                 auto ptr = mge::malloc(1024 * 1024);
                 mge::free(ptr);
             })
        .run("alloc_free_10M",
             [&]() {
                 auto ptr = mge::malloc(1024 * 1024 * 10);
                 mge::free(ptr);
             })
        .run("alloc_free_100M", [&]() {
            auto ptr = mge::malloc(1024 * 1024 * 100);
            mge::free(ptr);
        });
}

TEST(benchmark, pmr_new_delete_resource)
{
    mge::benchmark()
        .run("pmr_new_delete_64",
             [&]() {
                 auto* r = std::pmr::new_delete_resource();
                 void* p = r->allocate(64, alignof(std::max_align_t));
                 r->deallocate(p, 64, alignof(std::max_align_t));
             })
        .run("pmr_new_delete_512",
             [&]() {
                 auto* r = std::pmr::new_delete_resource();
                 void* p = r->allocate(512, alignof(std::max_align_t));
                 r->deallocate(p, 512, alignof(std::max_align_t));
             })
        .run("pmr_new_delete_4096", [&]() {
            auto* r = std::pmr::new_delete_resource();
            void* p = r->allocate(4096, alignof(std::max_align_t));
            r->deallocate(p, 4096, alignof(std::max_align_t));
        });
}

TEST(benchmark, pmr_monotonic_buffer_resource)
{
    // Buffer large enough so upstream is never hit during a single iteration.
    static constexpr size_t               buffer_size = 1024 * 1024;
    alignas(std::max_align_t) static char buf[buffer_size];

    mge::benchmark()
        .run("pmr_monotonic_64",
             [&]() {
                 std::pmr::monotonic_buffer_resource r(
                     buf,
                     sizeof(buf),
                     std::pmr::null_memory_resource());
                 void* p = r.allocate(64, alignof(std::max_align_t));
                 mge::do_not_optimize_away(p);
             })
        .run("pmr_monotonic_512",
             [&]() {
                 std::pmr::monotonic_buffer_resource r(
                     buf,
                     sizeof(buf),
                     std::pmr::null_memory_resource());
                 void* p = r.allocate(512, alignof(std::max_align_t));
                 mge::do_not_optimize_away(p);
             })
        .run("pmr_monotonic_4096", [&]() {
            std::pmr::monotonic_buffer_resource r(
                buf,
                sizeof(buf),
                std::pmr::null_memory_resource());
            void* p = r.allocate(4096, alignof(std::max_align_t));
            mge::do_not_optimize_away(p);
        });
}

TEST(benchmark, pmr_unsynchronized_pool_resource)
{
    // Pool is created once and reused across iterations to measure
    // steady-state free-list performance after initial slab allocation.
    std::pmr::unsynchronized_pool_resource pool(
        std::pmr::pool_options{0, 4096},
        std::pmr::new_delete_resource());

    mge::benchmark()
        .run("pmr_pool_64",
             [&]() {
                 void* p = pool.allocate(64, alignof(std::max_align_t));
                 pool.deallocate(p, 64, alignof(std::max_align_t));
             })
        .run("pmr_pool_512",
             [&]() {
                 void* p = pool.allocate(512, alignof(std::max_align_t));
                 pool.deallocate(p, 512, alignof(std::max_align_t));
             })
        .run("pmr_pool_4096", [&]() {
            void* p = pool.allocate(4096, alignof(std::max_align_t));
            pool.deallocate(p, 4096, alignof(std::max_align_t));
        });
}

TEST(benchmark, pmr_synchronized_pool_resource)
{
    // Same setup as unsynchronized — single pool reused across iterations
    // to measure the mutex overhead vs. the unsynchronized variant.
    std::pmr::synchronized_pool_resource pool(std::pmr::pool_options{0, 4096},
                                              std::pmr::new_delete_resource());

    mge::benchmark()
        .run("pmr_sync_pool_64",
             [&]() {
                 void* p = pool.allocate(64, alignof(std::max_align_t));
                 pool.deallocate(p, 64, alignof(std::max_align_t));
             })
        .run("pmr_sync_pool_512",
             [&]() {
                 void* p = pool.allocate(512, alignof(std::max_align_t));
                 pool.deallocate(p, 512, alignof(std::max_align_t));
             })
        .run("pmr_sync_pool_4096", [&]() {
            void* p = pool.allocate(4096, alignof(std::max_align_t));
            pool.deallocate(p, 4096, alignof(std::max_align_t));
        });
}