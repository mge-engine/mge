# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
FIND_PATH(linenoise_INCLUDE_DIR NAMES linenoise.h)

FILE(GLOB_RECURSE __linenoise_cpp_location "${VCPKG_INSTALLED_DIR}/../buildtrees/**/linenoise.cpp")
GET_FILENAME_COMPONENT(__linenoise_source_dir ${__linenoise_cpp_location} DIRECTORY)
SET(linenoise_SOURCES
    ${__linenoise_source_dir}/linenoise.cpp
    ${__linenoise_source_dir}/ConvertUTF.cpp
    ${__linenoise_source_dir}/wcwidth.cpp
)

ADD_LIBRARY(linenoise
            STATIC
            ${linenoise_SOURCES})
TARGET_INCLUDE_DIRECTORIES(linenoise PUBLIC ${linenoise_INCLUDE_DIR})
TARGET_COMPILE_FEATURES(linenoise PRIVATE cxx_std_17)
IF(MSVC)
    TARGET_COMPILE_OPTIONS(linenoise PRIVATE -wd5052 -wd4244 -wd4242 -wd4245 -wd4505 -wd4701)
ENDIF()


