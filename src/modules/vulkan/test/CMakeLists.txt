# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(vulkantest
	test_main.cpp
	vulkan_test.cpp
)
TARGET_LINK_LIBRARIES(vulkantest mgegraphics mgecore gmock)
TARGET_IS_TEST(vulkantest NEEDSDISPLAY)
