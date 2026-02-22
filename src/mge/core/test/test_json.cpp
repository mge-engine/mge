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

TEST(json, to_json_char_pointer)
{
    mge::json::json doc;
    const char*     v = "value";
    mge::to_json(doc["wurst"], v);
    EXPECT_STREQ("{\"wurst\":\"value\"}", doc.dump().c_str());
}

TEST(json, to_json_map)
{
    std::map<std::string, std::string> values = {{"key1", "value1"},
                                                 {"key2", "value2"},
                                                 {"key3", "value3"}};

    mge::json::json js;
    mge::to_json(js, values);
    EXPECT_EQ(3u, js.size());
    EXPECT_EQ("value1", js["key1"]);
    EXPECT_EQ("value2", js["key2"]);
    EXPECT_EQ("value3", js["key3"]);
}

TEST(json, to_json_vector)
{
    std::vector<int> values = {1, 2, 3};
    mge::json::json  js;
    mge::to_json(js, values);
    EXPECT_EQ(3u, js.size());
    EXPECT_EQ(1, js[0]);
    EXPECT_EQ(2, js[1]);
    EXPECT_EQ(3, js[2]);
}

TEST(json, to_json_string)
{
    mge::json::json doc;
    std::string     v = "value";
    mge::to_json(doc["key"], v);
    EXPECT_STREQ("{\"key\":\"value\"}", doc.dump().c_str());
}

TEST(json, to_json_string_view)
{
    mge::json::json  doc;
    std::string_view v = "value";
    mge::to_json(doc["key"], v);
    EXPECT_STREQ("{\"key\":\"value\"}", doc.dump().c_str());
}