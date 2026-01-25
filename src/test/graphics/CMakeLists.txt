# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

# Copy Python test script to build directory
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/capturetest.py"
    "${CMAKE_BINARY_DIR}/capturetest.py"
    COPYONLY
)

# Register Python-based RenderDoc capture tests
# Note: These tests require display and will be skipped in headless environments
IF(FALSE)
    ADD_TEST(
        NAME test_blackscreen_capture_dx11
        COMMAND "${Python3_EXECUTABLE}" 
            "${CMAKE_BINARY_DIR}/test_blackscreen_capture.py"
            --blackscreen-exe "$<TARGET_FILE:blackscreen>"
            --renderdoccmd "${RENDERDOCCMD_EXECUTABLE}"
            --sanitize-script "${SANITIZE_SCRIPT}"
            --compare-script "${COMPARE_SCRIPT}"
            --golden-dir "${GOLDEN_DIR}"
            --frame-count ${MGE_CAPTURE_FRAME_COUNT}
            BlackscreenCaptureTest.test_dx11
    )
    
    ADD_TEST(
        NAME test_blackscreen_capture_dx12
        COMMAND "${Python3_EXECUTABLE}" 
            "${CMAKE_BINARY_DIR}/test_blackscreen_capture.py"
            --blackscreen-exe "$<TARGET_FILE:blackscreen>"
            --renderdoccmd "${RENDERDOCCMD_EXECUTABLE}"
            --sanitize-script "${SANITIZE_SCRIPT}"
            --compare-script "${COMPARE_SCRIPT}"
            --golden-dir "${GOLDEN_DIR}"
            --frame-count ${MGE_CAPTURE_FRAME_COUNT}
            BlackscreenCaptureTest.test_dx12
    )
    
    ADD_TEST(
        NAME test_blackscreen_capture_opengl
        COMMAND "${Python3_EXECUTABLE}" 
            "${CMAKE_BINARY_DIR}/test_blackscreen_capture.py"
            --blackscreen-exe "$<TARGET_FILE:blackscreen>"
            --renderdoccmd "${RENDERDOCCMD_EXECUTABLE}"
            --sanitize-script "${SANITIZE_SCRIPT}"
            --compare-script "${COMPARE_SCRIPT}"
            --golden-dir "${GOLDEN_DIR}"
            --frame-count ${MGE_CAPTURE_FRAME_COUNT}
            BlackscreenCaptureTest.test_opengl
    )
    
    ADD_TEST(
        NAME test_blackscreen_capture_vulkan
        COMMAND "${Python3_EXECUTABLE}" 
            "${CMAKE_BINARY_DIR}/test_blackscreen_capture.py"
            --blackscreen-exe "$<TARGET_FILE:blackscreen>"
            --renderdoccmd "${RENDERDOCCMD_EXECUTABLE}"
            --sanitize-script "${SANITIZE_SCRIPT}"
            --compare-script "${COMPARE_SCRIPT}"
            --golden-dir "${GOLDEN_DIR}"
            --frame-count ${MGE_CAPTURE_FRAME_COUNT}
            BlackscreenCaptureTest.test_vulkan
    )
ENDIF()
