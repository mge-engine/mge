# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_TEST(
    TARGET      test_executable_name
    SOURCES     test_executable_name.cpp
    LIBRARIES   mgecore
)

MGE_TEST(
    TARGET      test_type_name
    SOURCES     test_type_name.cpp
    LIBRARIES   mgecore
)
