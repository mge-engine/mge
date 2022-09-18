// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/proxy.hpp"
#include "test/googletest.hpp"

#pragma warning(disable : 4003)
namespace mge::script {

    class ptest1
    {
    public:
        virtual ~ptest1() = default;
        virtual int  foo(int x) const = 0;
        virtual void no_arg_method() = 0;
    };

    class ptest1_proxy : public mge::script::proxy<ptest1>
    {
    public:
        virtual ~ptest1_proxy() = default;

        MGESCRIPT_PROXY_METHOD(int, foo, (int), (const));
        MGESCRIPT_PROXY_METHOD(void, no_arg_method, (), ());
    };

} // namespace mge::script

TEST(test_preprocessor, check_empty)
{
    auto x = BOOST_PP_VARIADIC_HAS_OPT();
    EXPECT_EQ(0, x);
}