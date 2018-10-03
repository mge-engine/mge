/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "mge/core/log_severity.hpp"
#ifdef MGE_OS_WINDOWS
#  include <windows.h>
#endif

using namespace mge;

TEST(log_severity, constants)
{
    EXPECT_EQ(log_severity::INFO, log_severity::INFO_SEVERITY);
#ifndef MGE_OS_WINDOWS
    EXPECT_EQ(log_severity::ERROR, log_severity::ERROR_SEVERITY);
#endif
    EXPECT_EQ(log_severity::WARNING, log_severity::WARNING_SEVERITY);
    EXPECT_EQ(log_severity::DEBUG, log_severity::DEBUG_SEVERITY);
}


