# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEENTITY_TEST_SOURCES
    test_schema.cpp
)

MGE_TEST(
    TARGET      test_entity
    SOURCES     ${MGEENTITY_TEST_SOURCES}
    LIBRARIES   mgecore mgeentity
)

FIND_PACKAGE(EnTT CONFIG REQUIRED)

MGE_TEST(
    TARGET      bench_ecs
    SOURCES     bench_ecs.cpp
    LIBRARIES   mgecore mgeentity benchmark EnTT::EnTT
)

find_package(flecs CONFIG REQUIRED)
target_link_libraries(bench_ecs $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>)