// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "test/googletest.hpp"

namespace mge {

    enum test_enum
    {
        ENUM_VALUE1,
        ENUM_VALUE2
    };

    struct test_struct
    {
        int   field1;
        bool  field2;
        float field3;
    };

    TEST(module, simple)
    {
        /*
        mge::script::module("mge")(
            type<test_enum>()
                .enum_value("ENUM_VALUE1", ENUM_VALUE1)
                .enum_value("ENUM_VALUE2", ENUM_VALUE2),
            type<test_struct>().field("field1", &test_struct::field1),
            module("foobar")(

                ));
        */
    }

} // namespace mge
