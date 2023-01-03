# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  benchmark
    TYPE    STATIC
    SOURCES
        benchmark.cpp)

ADD_SUBDIRECTORY(test)