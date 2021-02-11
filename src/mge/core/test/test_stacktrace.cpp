// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googletest.hpp"
#include <boost/stacktrace/stacktrace.hpp>

TEST(stacktrace, basic)
{
    auto t = boost::stacktrace::stacktrace();
    std::cout << t;
    /* for (const auto &f : t) {
        std::cout << f.address() << " " << f.name() << " " << f.source_file()
                  << ":" << f.source_line() << std::endl;
    }*/
}
