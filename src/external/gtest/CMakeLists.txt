# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(gtest "${CMAKE_SOURCE_DIR}/src/external/gtest/src/gtest-all.cc")

TARGET_INCLUDE_DIRECTORIES(gtest
PUBLIC  "${CMAKE_SOURCE_DIR}/src/external/gtest/include"
PRIVATE "${CMAKE_SOURCE_DIR}/src/external/gtest")

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  TARGET_LINK_LIBRARIES(gtest pthread)
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
    SET_SOURCE_FILES_PROPERTIES("${CMAKE_SOURCE_DIR}/src/external/gtest/src/gtest-all.cc"
        PROPERTIES COMPILE_FLAGS "-Wno-error")
ENDIF()

IF(MSVC)
    FOREACH(FLAG_VAR
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        STRING(REPLACE "/MT" "-MD" ${FLAG_VAR} "${${FLAG_VAR}}")
        STRING(REPLACE "-MT" "-MD" ${FLAG_VAR} "${${FLAG_VAR}}")
    ENDFOREACH()
ENDIF()
