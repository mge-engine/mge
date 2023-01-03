# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(GTest CONFIG REQUIRED)

ADD_LIBRARY(googletest ALIAS GTest::gtest)
ADD_LIBRARY(googlemock ALIAS GTest::gmock)