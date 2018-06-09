# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(gmock "${CMAKE_SOURCE_DIR}/src/external/gmock/src/gmock-all.cc")

TARGET_INCLUDE_DIRECTORIES(gmock
PUBLIC  "${CMAKE_SOURCE_DIR}/src/external/gtest/include"
PUBLIC  "${CMAKE_SOURCE_DIR}/src/external/gmock/include"
PRIVATE "${CMAKE_SOURCE_DIR}/src/external/gmock")

IF(CMAKE_COMPILER_IS_GNUCC)
    SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/src/external/gmock/src/gmock-all.cc"
        PROPERTIES COMPILE_FLAGS "-Wno-error")
ENDIF()

TARGET_LINK_LIBRARIES(gmock gtest)

IF(MSVC)
    FOREACH(FLAG_VAR
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING(REPLACE "/MT" "-MD" ${FLAG_VAR} "${${FLAG_VAR}}")
        STRING(REPLACE "-MT" "-MD" ${FLAG_VAR} "${${FLAG_VAR}}")
    ENDFOREACH()
ENDIF()

