// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "test/benchmark.hpp"
#include "test/googletest.hpp"

#include "mge/core/memory.hpp"

TEST(benchmark, malloc_free)
{
    mge::benchmark()
        .show_results()
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