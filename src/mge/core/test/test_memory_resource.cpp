// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

TEST(memory_source, allocate_shared)
{
    auto stringptr =
        std::allocate_shared<std::string>(mge::memory_resource::allocator,
                                          "Hello, World!");
}

TEST(named_memory_resource, statistics_tracked)
{
    mge::named_memory_resource r("test_pool"sv);

    auto& s = r.resource_statistics();
    EXPECT_EQ("test_pool"sv, s.name());

    const auto& desc = s.describe();
    // locate fields
    auto alloc_idx = static_cast<mge::statistics::description::size_type>(-1);
    auto dealloc_idx = alloc_idx;
    auto bytes_idx = alloc_idx;
    for (mge::statistics::description::size_type i = 0; i < desc.size(); ++i) {
        if (desc.at(i).name() == "allocations"sv)
            alloc_idx = i;
        if (desc.at(i).name() == "deallocations"sv)
            dealloc_idx = i;
        if (desc.at(i).name() == "allocated_bytes"sv)
            bytes_idx = i;
    }
    ASSERT_NE(alloc_idx,
              static_cast<mge::statistics::description::size_type>(-1));
    ASSERT_NE(dealloc_idx,
              static_cast<mge::statistics::description::size_type>(-1));
    ASSERT_NE(bytes_idx,
              static_cast<mge::statistics::description::size_type>(-1));

    // initial counts are zero
    EXPECT_EQ((mge::statistics::value_type{uint64_t{0}}),
              desc.at(alloc_idx).get(s));
    EXPECT_EQ((mge::statistics::value_type{uint64_t{0}}),
              desc.at(dealloc_idx).get(s));

    // perform one allocation + deallocation
    void* p = r.allocate(64, alignof(std::max_align_t));
    EXPECT_EQ((mge::statistics::value_type{uint64_t{1}}),
              desc.at(alloc_idx).get(s));
    EXPECT_GE(std::get<uint64_t>(desc.at(bytes_idx).get(s)), uint64_t{64});

    r.deallocate(p, 64, alignof(std::max_align_t));
    EXPECT_EQ((mge::statistics::value_type{uint64_t{1}}),
              desc.at(dealloc_idx).get(s));
}
