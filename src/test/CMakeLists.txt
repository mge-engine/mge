# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  benchmark
    TYPE    STATIC
    SOURCES
        benchmark.cpp)
TARGET_LINK_LIBRARIES(benchmark PRIVATE nanobench::nanobench)
ADD_SUBDIRECTORY(test)

# RenderDoc capture validation tests
FIND_PACKAGE(Python3 COMPONENTS Interpreter)

IF(RENDERDOCCMD_EXECUTABLE AND Python3_FOUND AND NOT MGE_HEADLESS)
    SET(MGE_CAPTURE_FRAME_COUNT 5 CACHE STRING "Number of frames to capture in RenderDoc tests")
    
    # Find blackscreen executable
    SET(BLACKSCREEN_EXE "$<TARGET_FILE:blackscreen>")
    
    # Python scripts for capture processing
    SET(SANITIZE_SCRIPT "${CMAKE_SOURCE_DIR}/tools/rdoc/sanitize_capture.py")
    SET(COMPARE_SCRIPT "${CMAKE_SOURCE_DIR}/tools/rdoc/compare_captures.py")
    SET(GOLDEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/renderdoc_captures/blackscreen")
    
    # Create test subdirectory
    ADD_SUBDIRECTORY(graphics)
ENDIF()