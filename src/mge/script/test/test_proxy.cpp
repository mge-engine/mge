// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/proxy.hpp"
#include "mock_invoke_context.hpp"
#include "test/googletest.hpp"

#pragma warning(disable : 4003)

using namespace testing;

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

        MGESCRIPT_PROXY_METHOD(int, foo, (int), (const))
        MGESCRIPT_PROXY_METHOD(void, no_arg_method, (), ())
    };

} // namespace mge::script

TEST(proxy, simple_call)
{
    mge::script::ptest1_proxy px;
    MOCK_invoke_context       ctx;

    EXPECT_CALL(ctx, call(_)).Times(1);

    px.set_invoke_context(&ctx);
    px.no_arg_method();
}