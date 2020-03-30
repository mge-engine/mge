// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log_formatter.hpp"
#include "mge/core/log_record.hpp"
#include "test/googletest.hpp"
#include <thread>

using namespace mge;

TEST(text_log_formatter, can_be_instantiated)
{
    log_formatter_ref formatter = log_formatter::create("text");
    ASSERT_TRUE(formatter.get() != nullptr);
}
