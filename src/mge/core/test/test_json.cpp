// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/json.hpp"
#include "test/googletest.hpp"

TEST(json, empty)
{
    mge::json::json doc;
    EXPECT_TRUE(doc.empty());
}

TEST(json, from_json_string)
{
    mge::json::json doc;
    doc["key"] = "value";
    std::string v;
    mge::from_json(doc["key"], v);
    EXPECT_EQ("value", v);
}

TEST(json, from_json_string_int)
{
    mge::json::json doc;
    doc["key"] = 65536;
    std::string v;
    mge::from_json(doc["key"], v);
    EXPECT_EQ("65536", v);
}

TEST(json, from_json_string_boolean)
{
    mge::json::json doc;
    doc["key"] = true;
    std::string v;
    mge::from_json(doc["key"], v);
    EXPECT_EQ("true", v);
}

TEST(json, from_json_vector)
{
    mge::json::json doc;
    mge::json::json array = mge::json::json::array();
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    doc["key"] = array;
    std::vector<int> values;
    mge::from_json(doc["key"], values);
    EXPECT_EQ(3u, values.size());
    EXPECT_EQ(1, values[0]);
    EXPECT_EQ(2, values[1]);
    EXPECT_EQ(3, values[2]);
}

TEST(json, from_json_map)
{
    mge::json::json js = {{"key1", "value1"},
                          {"key2", "value2"},
                          {"key3", "value3"}};

    std::map<std::string, std::string> values;
    mge::from_json(js, values);
    EXPECT_EQ(3u, values.size());
}