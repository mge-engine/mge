// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "test/googletest.hpp"
#include <vector>

TEST(memory_resource, tracing_resource) {
    mge::tracing_memory_resource mem(std::pmr::new_delete_resource());
    std::pmr::vector<int> v(&mem);

    for(int i=0; i<100; ++i) {
        v.push_back(i);
    }
}

TEST(memory_resource, callback_resource) {
    bool allocate_callback_called = false;
    bool deallocate_callback_called = false;
    mge::callback_memory_resource mem(std::pmr::new_delete_resource());
    mem.set_allocate_callback([&](size_t bytes, size_t align, void *ptr){
        allocate_callback_called = true;
    });
    mem.set_deallocate_callback([&](size_t bytes, size_t align, void *ptr){
        deallocate_callback_called = true;
    });
    {
        std::pmr::vector<int> v(&mem);
        v.push_back(100);
    }
    EXPECT_TRUE(allocate_callback_called);
    EXPECT_TRUE(deallocate_callback_called);
}