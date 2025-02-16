// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/implement_method.hpp"
#include "mge/script/invocation_context.hpp"
#include "mge/script/module.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/type.hpp"
#include "test/googletest.hpp"

class test_interface
{
public:
    virtual ~test_interface() = default;
    virtual int  test_method(int a, int b) = 0;
    virtual void test_void_method() = 0;
};

class test_proxy : public mge::script::proxy<test_interface>
{
public:
    test_proxy() = default;

    MGE_IMPLEMENT_METHOD(int, test_method, (int, int), override);
};

// removes from type space name the type and space

TEST(proxy, preproc) {}
