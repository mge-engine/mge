#include "mge/entity/schema.hpp"
#include "test/benchmark.hpp"
#include "test/googletest.hpp"

TEST(benchmark, startup_shutdown)
{
    mge::benchmark().show_results().run("mge_startup_shutdown",
                                        [&]() { mge::entity::schema schema; });
}

TEST(benchmark, entity_create_destroy) {}

TEST(benchmark, entity_add_parent) {}

TEST(benchmark, entity_add_child) {}

TEST(benchmark, entity_add_component) {}

TEST(benchmark, entity_remove_component) {}