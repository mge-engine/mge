// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/component.hpp"

using mge::component;

class a_component: public mge::component<a_component>
{
public:
    virtual ~a_component() = default;
    virtual int foo() = 0;
};

MGE_REGISTER_COMPONENT(a_component);

TEST( component, registered )
{
    EXPECT_TRUE(mge::component<a_component>::component_registered("a_component"));
}

class a_implementation: public a_component
{
public:
    virtual ~a_implementation() = default;

    int foo()
    {
        return 42;
    }
};

MGE_REGISTER_IMPLEMENTATION(a_implementation, a_component, adummy, bdummy);

TEST( implementation, registered )
{
    EXPECT_TRUE(component<a_component>::implementation_registered("a_component", "a_implementation"));
    EXPECT_TRUE(component<a_component>::implementation_registered("a_component", "adummy"));
    EXPECT_TRUE(component<a_component>::implementation_registered("a_component", "bdummy"));
    EXPECT_FALSE(component<a_component>::implementation_registered("a_component", "   bdummy   "));
    EXPECT_FALSE(component<a_component>::implementation_registered("a_component", "ADUMMY"));
}

TEST( component, implementations )
{
    std::vector<std::string> implementations;
    component<a_component>::implementations([&](const std::string& s) {
        implementations.emplace_back(s);
    });
    ASSERT_EQ(1u, implementations.size());
    EXPECT_EQ("a_implementation", implementations[0]);
}


TEST( implementation, create )
{
    std::shared_ptr<a_component> c =a_component::create("a_implementation");
    ASSERT_FALSE(c == 0);
    EXPECT_EQ(42, c->foo());
}

TEST( implementation, create_with_alias )
{
    std::shared_ptr<a_component> c =a_component::create("adummy");
    ASSERT_FALSE(c == 0);
    EXPECT_EQ(42, c->foo());
}
