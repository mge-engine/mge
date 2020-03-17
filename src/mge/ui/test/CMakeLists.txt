# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
IF(MSVC)
ADD_EXECUTABLE(stb_truetype_test
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_stb_truetype.cpp)
TARGET_LINK_LIBRARIES(stb_truetype_test mgecore mgemath mgeui gmock freetype)
TARGET_IS_TEST(stb_truetype_test)
ENDIF()
