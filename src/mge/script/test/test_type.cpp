// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include "mock_call_context.hpp"
#include "test/googletest.hpp"

using namespace testing;

static bool test_class_ctor_called{false};
static bool test_class_dtor_called{false};

struct test_class
{
    test_class() { test_class_ctor_called = true; }

    ~test_class() { test_class_dtor_called = true; }
};

TEST(type, ctor_dtor)
{
    using namespace mge::script;

    mge::script::module m;
    m(type<test_class>());

    const auto& t = m.type("test_class");
    EXPECT_EQ("test_class", t.name());
    EXPECT_TRUE(t.class_specific().default_construct);
    EXPECT_TRUE(t.class_specific().destroy);
    EXPECT_EQ(1, t.class_specific().size);

    MOCK_call_context ctx;
    void*             data = mge::malloc(1);

    EXPECT_CALL(ctx, get_this()).WillRepeatedly(Return(data));
    t.class_specific().default_construct(ctx);
    EXPECT_TRUE(test_class_ctor_called);
    test_class_ctor_called = false;
    t.class_specific().destroy(ctx);
    EXPECT_TRUE(test_class_dtor_called);
    test_class_dtor_called = false;
    mge::free(data);
}
