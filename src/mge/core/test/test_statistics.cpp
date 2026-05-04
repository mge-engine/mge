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

TEST(statistics, children)
{
    mge::statistics parent("parent_node"sv);
    mge::statistics child1(parent, "child_a"sv);
    mge::statistics child2(parent, "child_b"sv);

    auto span = parent.children();
    ASSERT_EQ(2u, span.size());
    EXPECT_EQ("child_a"sv, span[0]->name());
    EXPECT_EQ("child_b"sv, span[1]->name());

    child2.release();
    child1.release();
    parent.release();
}

TEST(statistics, visit_traversal)
{
    mge::statistics root_node("visit_root"sv);
    mge::statistics child1(root_node, "v_child_a"sv);
    mge::statistics child2(root_node, "v_child_b"sv);
    mge::statistics grandchild(child1, "v_grandchild"sv);

    std::vector<std::pair<std::string, int>> visited;
    root_node.visit([&](const mge::statistics& s, int depth) {
        visited.emplace_back(std::string(s.name()), depth);
    });

    ASSERT_EQ(4u, visited.size());
    EXPECT_EQ("visit_root"sv, visited[0].first);
    EXPECT_EQ(0, visited[0].second);
    EXPECT_EQ("v_child_a"sv, visited[1].first);
    EXPECT_EQ(1, visited[1].second);
    EXPECT_EQ("v_grandchild"sv, visited[2].first);
    EXPECT_EQ(2, visited[2].second);
    EXPECT_EQ("v_child_b"sv, visited[3].first);
    EXPECT_EQ(1, visited[3].second);

    grandchild.release();
    child2.release();
    child1.release();
    root_node.release();
}

TEST(statistics, get_const)
{
    my_stats                    s;
    const my_stats&             cs = s;
    mge::statistics::value_type val = s.describe().at(0).get(cs);
    EXPECT_EQ((mge::statistics::value_type{uint64_t{17}}), val);
    s.release();
}