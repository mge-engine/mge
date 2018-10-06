/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "mge/core/configuration.hpp"

#include <fstream>

using mge::configuration;
using mge::configuration_key_not_found;

class configuration_test : public ::testing::Test
{
public:
    void SetUp()
    {
        configuration c;
        c.clear();
    }
};

TEST_F(configuration_test, empty_constructed)
{
    configuration c;
    EXPECT_EQ(true, c.empty());
}

TEST_F(configuration_test, simple_store)
{
    configuration c;
    c.set("configuration_test.key", "value");
    c.store();
}

TEST_F(configuration_test, retrieve)
{
    {
        configuration c;
        c.set("configuration_test.key", "value");
        c.store();
    }
    {
        configuration c;
        std::string value = c.value("configuration_test.key");
        EXPECT_EQ(std::string("value"), value);
    }
}

TEST_F(configuration_test, retrieve_string)
{
    {
        configuration c;
        c.set("configuration_test.key", "value");
        c.store();
    }
    {
        configuration c;
        std::string key("configuration_test.key");
        std::string value = c.value(key);
        EXPECT_EQ(std::string("value"), value);
    }
}


TEST_F(configuration_test, bool_retrieve)
{
    {
        configuration c;
        c.set("configuration_test.key", "true");
        c.store();
    }
    {
        configuration c;
        bool value = c.value<bool>("configuration_test.key", false);
        EXPECT_TRUE(value);
    }
}

TEST_F(configuration_test, bool_retrieve_string_key)
{
    {
        configuration c;
        c.set("configuration_test.key", "true");
        c.store();
    }
    {
        configuration c;
        std::string key("configuration_test.key");
        bool value = c.value<bool>(key, false);
        EXPECT_TRUE(value);
    }
}


TEST_F(configuration_test, bool_retrieve_no_default)
{
    {
        configuration c;
        c.set("configuration_test.key", "true");
        c.store();
    }
    {
        configuration c;
        bool value = c.value<bool>("configuration_test.key");
        EXPECT_TRUE(value);
    }
}


TEST_F(configuration_test, bool_retrieve_no_default_string_key)
{
    {
        configuration c;
        c.set("configuration_test.key", "true");
        c.store();
    }
    {
        configuration c;
        std::string key("configuration_test.key");
        bool value = c.value<bool>(key);
        EXPECT_TRUE(value);
    }
}



TEST_F(configuration_test, list_retrieve)
{
    {
        configuration c;
        c.set("configuration_test.key", "value1,value2");
        c.store();
    }
    {
        configuration c;
        auto listvalue = c.list_value("configuration_test.key");
        ASSERT_EQ(2u, listvalue.size());
        EXPECT_EQ(std::string("value1"), listvalue[0]);
        EXPECT_EQ(std::string("value2"), listvalue[1]);
    }
}

TEST_F(configuration_test, simple_contains_key)
{
    {
        configuration c;
        c.set("configuration_test.key", "value");
        c.store();
    }
    {
        configuration c;
        EXPECT_TRUE(c.contains_key("configuration_test.key"));
    }
}

TEST_F(configuration_test, get_keys)
{
    {
        configuration c;
        c.set("configuration_test.keyA", "value");
        c.set("configuration_test.keyB", "value");
        c.store();
    }
    {
        configuration c;
        configuration::key_list l = c.keys();
        EXPECT_EQ(std::string("configuration_test.keyA"), l[0]);
        EXPECT_EQ(std::string("configuration_test.keyB"), l[1]);
    }
}

TEST_F( configuration_test, get_non_existent )
{
    configuration c;
    EXPECT_THROW(c.value<std::string>("anonexistentkey"),
                 configuration_key_not_found);
}

TEST_F( configuration_test, contains_key_for_non_existent )
{
    configuration c;
    EXPECT_FALSE(c.contains_key("anonexistentkey"));
}


TEST_F( configuration_test, clear )
{
    configuration c;
    c.set("configuration_test.key", "value");
    EXPECT_FALSE(c.empty());
    c.clear();
    EXPECT_TRUE(c.empty());
}

TEST_F( configuration_test, clear_clears_persistence )
{
    configuration c;
    c.set("configuration_test.key", "value");
    EXPECT_FALSE(c.empty());
    c.clear();
    configuration c2;
    EXPECT_TRUE(c2.empty());
}



