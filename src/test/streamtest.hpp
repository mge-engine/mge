// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include <sstream>

#define EXPECT_STREAMOUTPUT(STRING, VALUE)                                     \
    [&]() {                                                                    \
        std::stringstream ss;                                                  \
        ss << VALUE;                                                           \
        EXPECT_STREQ(STRING, ss.str().c_str());                                \
    }()
