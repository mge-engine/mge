// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/component.hpp"
#include "test/googletest.hpp"

using mge::component;

class a_component : public mge::component<a_component>
{
public:
    virtual ~a_component() = default;
    virtual int foo() = 0;
};

MGE_REGISTER_COMPONENT(a_component);

TEST(component, registered)
{
    EXPECT_TRUE(
        mge::component<a_component>::component_registered("a_component"));
}

class a_implementation : public a_component
{
public:
    virtual ~a_implementation() = default;

    int foo()
    {
        return 42;
    }
};

MGE_REGISTER_IMPLEMENTATION(a_implementation, a_component, adummy, bdummy);

TEST(implementation, registered)
{
    EXPECT_TRUE(
        component<a_component>::implementation_registered("a_component",
                                                          "a_implementation"));
    EXPECT_TRUE(component<a_component>::implementation_registered("a_component",
                                                                  "adummy"));
    EXPECT_TRUE(component<a_component>::implementation_registered("a_component",
                                                                  "bdummy"));
    EXPECT_FALSE(
        component<a_component>::implementation_registered("a_component",
                                                          "   bdummy   "));
    EXPECT_FALSE(
        component<a_component>::implementation_registered("a_component",
                                                          "ADUMMY"));
}

TEST(component, implementations)
{
    std::vector<std::string> implementations;
    component<a_component>::implementations(
        [&](std::string_view s) { implementations.emplace_back(s); });
    ASSERT_EQ(1u, implementations.size());
    EXPECT_EQ("a_implementation", implementations[0]);
}

TEST(implementation, create)
{
    std::shared_ptr<a_component> c = a_component::create("a_implementation");
    ASSERT_FALSE(c == 0);
    EXPECT_EQ(42, c->foo());
}

TEST(implementation, create_with_alias)
{
    std::shared_ptr<a_component> c = a_component::create("adummy");
    ASSERT_FALSE(c == 0);
    EXPECT_EQ(42, c->foo());
}

class dynamic_impl : public a_component
{
public:
    int foo() override
    {
        return 99;
    }
};

TEST(dynamic_implementation, register_and_create)
{
    mge::dynamic_implementation_registry_entry entry(
        []() -> std::shared_ptr<mge::component_base> {
            return std::make_shared<dynamic_impl>();
        },
        "a_component",
        "dynamic_impl",
        "dynalias1",
        "dynalias2");

    EXPECT_TRUE(mge::component_base::implementation_registered("a_component",
                                                               "dynamic_impl"));
    auto c = a_component::create("dynamic_impl");
    ASSERT_TRUE(c);
    EXPECT_EQ(99, c->foo());
    entry.unregister();
}

TEST(dynamic_implementation, create_with_alias)
{
    mge::dynamic_implementation_registry_entry entry(
        []() -> std::shared_ptr<mge::component_base> {
            return std::make_shared<dynamic_impl>();
        },
        "a_component",
        "dynamic_impl2",
        "dalias1",
        "dalias2");

    auto c1 = a_component::create("dalias1");
    ASSERT_TRUE(c1);
    EXPECT_EQ(99, c1->foo());

    auto c2 = a_component::create("dalias2");
    ASSERT_TRUE(c2);
    EXPECT_EQ(99, c2->foo());
    entry.unregister();
}

TEST(dynamic_implementation, register_with_alias_string)
{
    mge::dynamic_implementation_registry_entry entry(
        []() -> std::shared_ptr<mge::component_base> {
            return std::make_shared<dynamic_impl>();
        },
        "a_component",
        "dynamic_impl3",
        std::string_view("salias1, salias2"));

    EXPECT_TRUE(mge::component_base::implementation_registered("a_component",
                                                               "salias1"));
    EXPECT_TRUE(mge::component_base::implementation_registered("a_component",
                                                               "salias2"));
    auto c = a_component::create("salias1");
    ASSERT_TRUE(c);
    EXPECT_EQ(99, c->foo());
    entry.unregister();
}

TEST(dynamic_implementation, unregister)
{
    {
        mge::dynamic_implementation_registry_entry entry(
            []() -> std::shared_ptr<mge::component_base> {
                return std::make_shared<dynamic_impl>();
            },
            "a_component",
            "dynamic_impl_unreg",
            "unreg_alias");

        EXPECT_TRUE(mge::component_base::implementation_registered(
            "a_component",
            "dynamic_impl_unreg"));
        EXPECT_TRUE(
            mge::component_base::implementation_registered("a_component",
                                                           "unreg_alias"));

        entry.unregister();

        EXPECT_FALSE(mge::component_base::implementation_registered(
            "a_component",
            "dynamic_impl_unreg"));
        EXPECT_FALSE(
            mge::component_base::implementation_registered("a_component",
                                                           "unreg_alias"));
    }
}
