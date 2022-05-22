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
        virtual int foo(int x) const = 0;
    };

    class ptest1_proxy : public mge::script::proxy<ptest1>
    {
    public:
        virtual ~ptest1_proxy() = default;

        MGESCRIPT_PROXY(int, foo, (int), (const))
    };

} // namespace mge::script