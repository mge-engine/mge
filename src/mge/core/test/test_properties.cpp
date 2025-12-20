// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/properties.hpp"
#include "test/googletest.hpp"
#include <sstream>

TEST(properties, default_constructor)
{
    mge::properties p;
    EXPECT_EQ(0, std::distance(p.begin(), p.end()));
}

TEST(properties, copy_constructor)
{
    mge::properties p1;
    p1.put("key1", "value1");
    p1.put("key2", 42);

    mge::properties p2(p1);
    EXPECT_EQ(p1, p2);
    EXPECT_TRUE(p2.exists("key1"));
    EXPECT_TRUE(p2.exists("key2"));
    EXPECT_EQ("value1", p2.get<std::string>("key1"));
    EXPECT_EQ(42, p2.get<int>("key2"));
}

TEST(properties, move_constructor)
{
    mge::properties p1;
    p1.put("key1", "value1");
    p1.put("key2", 42);

    mge::properties p2(std::move(p1));
    EXPECT_TRUE(p2.exists("key1"));
    EXPECT_TRUE(p2.exists("key2"));
    EXPECT_EQ("value1", p2.get<std::string>("key1"));
    EXPECT_EQ(42, p2.get<int>("key2"));
}

TEST(properties, load_from_stream)
{
    std::stringstream ss;
    ss << "key1=value1\n";
    ss << "key2=42\n";
    ss << "key3=3.14\n";

    mge::properties p(ss);
    EXPECT_TRUE(p.exists("key1"));
    EXPECT_TRUE(p.exists("key2"));
    EXPECT_TRUE(p.exists("key3"));
    EXPECT_EQ("value1", p.get<std::string>("key1"));
    EXPECT_EQ(42, p.get<int>("key2"));
    EXPECT_FLOAT_EQ(3.14f, p.get<float>("key3"));
}

TEST(properties, load_empty_stream)
{
    std::stringstream ss;
    mge::properties   p(ss);
    EXPECT_EQ(0, std::distance(p.begin(), p.end()));
}

TEST(properties, load_with_empty_lines)
{
    std::stringstream ss;
    ss << "key1=value1\n";
    ss << "\n";
    ss << "key2=value2\n";

    mge::properties p(ss);
    EXPECT_EQ(2, std::distance(p.begin(), p.end()));
    EXPECT_TRUE(p.exists("key1"));
    EXPECT_TRUE(p.exists("key2"));
}

TEST(properties, load_invalid_line)
{
    std::stringstream ss;
    ss << "invalid_line_without_equals\n";

    EXPECT_THROW(mge::properties p(ss), mge::exception);
}

TEST(properties, store_to_stream)
{
    mge::properties p;
    p.put("key1", "value1");
    p.put("key2", 42);
    p.put("key3", 3.14);

    std::stringstream ss;
    p.store(ss);

    std::string content = ss.str();
    EXPECT_NE(std::string::npos, content.find("key1=value1\n"));
    EXPECT_NE(std::string::npos, content.find("key2=42\n"));
    EXPECT_NE(std::string::npos, content.find("key3=3.14\n"));
}

TEST(properties, store_empty_properties)
{
    mge::properties   p;
    std::stringstream ss;
    p.store(ss);
    EXPECT_TRUE(ss.str().empty());
}

TEST(properties, load_and_store_roundtrip)
{
    mge::properties p1;
    p1.put("key1", "value1");
    p1.put("key2", 42);
    p1.put("key3", 3.14);

    std::stringstream ss;
    p1.store(ss);

    mge::properties p2;
    p2.load(ss);

    EXPECT_EQ(p1, p2);
    EXPECT_EQ("value1", p2.get<std::string>("key1"));
    EXPECT_EQ(42, p2.get<int>("key2"));
    EXPECT_FLOAT_EQ(3.14f, p2.get<float>("key3"));
}

TEST(properties, put_and_get)
{
    mge::properties p;
    p.put("string_key", "value");
    p.put("int_key", 123);
    p.put("float_key", 1.5f);

    EXPECT_EQ("value", p.get<std::string>("string_key"));
    EXPECT_EQ(123, p.get<int>("int_key"));
    EXPECT_FLOAT_EQ(1.5f, p.get<float>("float_key"));
}

TEST(properties, set_alias_for_put)
{
    mge::properties p;
    p.set("key", "value");
    EXPECT_EQ("value", p.get<std::string>("key"));
}

TEST(properties, get_with_default)
{
    mge::properties p;
    p.put("key1", 42);

    EXPECT_EQ(42, p.get<int>("key1", 0));
    EXPECT_EQ(99, p.get<int>("nonexistent", 99));
}

TEST(properties, get_nonexistent_throws)
{
    mge::properties p;
    EXPECT_THROW(p.get<std::string>("nonexistent"), mge::exception);
}

TEST(properties, exists)
{
    mge::properties p;
    p.put("key1", "value1");

    EXPECT_TRUE(p.exists("key1"));
    EXPECT_FALSE(p.exists("key2"));
    EXPECT_TRUE(p.exists(std::string("key1")));
    EXPECT_FALSE(p.exists(std::string("key2")));
}

TEST(properties, erase)
{
    mge::properties p;
    p.put("key1", "value1");
    p.put("key2", "value2");

    EXPECT_TRUE(p.exists("key1"));
    p.erase(std::string("key1"));
    EXPECT_FALSE(p.exists("key1"));
    EXPECT_TRUE(p.exists("key2"));

    p.erase(std::string("key2"));
    EXPECT_FALSE(p.exists("key2"));
}

TEST(properties, clear)
{
    mge::properties p;
    p.put("key1", "value1");
    p.put("key2", "value2");
    p.put("key3", "value3");

    EXPECT_EQ(3, std::distance(p.begin(), p.end()));
    p.clear();
    EXPECT_EQ(0, std::distance(p.begin(), p.end()));
    EXPECT_FALSE(p.exists("key1"));
}

TEST(properties, iteration)
{
    mge::properties p;
    p.put("key1", "value1");
    p.put("key2", "value2");
    p.put("key3", "value3");

    int count = 0;
    for (const auto& [key, value] : p) {
        EXPECT_FALSE(key.empty());
        EXPECT_FALSE(value.empty());
        ++count;
    }
    EXPECT_EQ(3, count);
}

TEST(properties, const_iteration)
{
    mge::properties p;
    p.put("key1", "value1");
    p.put("key2", "value2");

    const mge::properties& cp = p;
    int                    count = 0;
    for (auto it = cp.begin(); it != cp.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(2, count);
}

TEST(properties, equality)
{
    mge::properties p1;
    p1.put("key1", "value1");
    p1.put("key2", 42);

    mge::properties p2;
    p2.put("key1", "value1");
    p2.put("key2", 42);

    mge::properties p3;
    p3.put("key1", "value1");
    p3.put("key2", 43);

    EXPECT_EQ(p1, p2);
    EXPECT_NE(p1, p3);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p1 != p3);
}

TEST(properties, copy_assignment)
{
    mge::properties p1;
    p1.put("key1", "value1");

    mge::properties p2;
    p2 = p1;

    EXPECT_EQ(p1, p2);
    EXPECT_TRUE(p2.exists("key1"));
    EXPECT_EQ("value1", p2.get<std::string>("key1"));
}

TEST(properties, move_assignment)
{
    mge::properties p1;
    p1.put("key1", "value1");

    mge::properties p2;
    p2 = std::move(p1);

    EXPECT_TRUE(p2.exists("key1"));
    EXPECT_EQ("value1", p2.get<std::string>("key1"));
}
