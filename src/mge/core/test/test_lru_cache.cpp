#include "mge/core/lru_cache.hpp"
#include "test/googletest.hpp"

TEST(lru_cache, basic_functionality)
{
    // Create an LRU cache with a capacity of 3
    mge::lru_cache<int, int> cache(3);

    // Insert some key-value pairs
    cache.insert(1, 10);
    cache.insert(2, 20);
    cache.insert(3, 30);

    // Access some values
    EXPECT_EQ(cache.get(1), 10);
    EXPECT_EQ(cache.get(2), 20);
    EXPECT_EQ(cache.get(3), 30);

    // Insert another key-value pair, causing the cache to evict the least
    // recently used item
    cache.insert(4, 40);

    // The least recently used item (key=1) should have been evicted
    EXPECT_FALSE(cache.get(1).has_value());

    // The other items should still be accessible
    EXPECT_EQ(cache.get(2), 20);
    EXPECT_EQ(cache.get(3), 30);
    EXPECT_EQ(cache.get(4), 40);
}
