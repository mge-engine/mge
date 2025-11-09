// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/statistics.hpp"
#include "test/googletest.hpp"

using namespace std::string_view_literals;

TEST(statistics, create)
{
    mge::statistics s("foobar");

    EXPECT_EQ("foobar"sv, s.name());
    s.release();
}

TEST(statistics, create_from_string)
{
    mge::statistics s(std::string("foobar"));

    EXPECT_EQ("foobar"sv, s.name());
    s.release();
}

TEST(statistics, root_exists)
{
    EXPECT_EQ(""sv, mge::statistics::root().name());
}

TEST(statistics, death_on_destroy_owned)
{
    ASSERT_DEATH({ mge::statistics s("foobar"); }, ".*");
}

class my_stats : public mge::statistics
{
public:
    my_stats()
        : mge::statistics("my_stats"sv)
    {
        count = 17;
    }

    const description& describe() const override
    {
        static mge::statistics::description desc(
            "my_stats"sv,
            "my test statistics"sv,
            {mge::statistics::description::field("count"sv, &my_stats::count)});
        return desc;
    }

    statistics::counter_type count;
};

TEST(statistics, describe)
{
    my_stats s;
    EXPECT_EQ("my_stats"sv, s.describe().name());
    EXPECT_EQ("my test statistics"sv, s.describe().comment());
    EXPECT_EQ(1u, s.describe().size());
    EXPECT_EQ("count"sv, s.describe().at(0).name());
    mge::statistics::value_type val1(static_cast<uint64_t>(17));
    mge::statistics::value_type val2(s.describe().at(0).get(s));
    EXPECT_TRUE(std::holds_alternative<uint64_t>(val2));
    EXPECT_EQ(val1, val2);
    s.release();
}