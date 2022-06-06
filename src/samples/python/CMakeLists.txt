# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
ADD_CUSTOM_TARGET(python-samples
    ALL
    DEPENDS
    ${CMAKE_BINARY_DIR}/samples/python
    ${CMAKE_BINARY_DIR}/samples/python/blackscreen.py)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/samples/python
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/samples/python
)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_BINARY_DIR}/samples/python/blackscreen.py
    DEPENDS blackscreen.py
            ${CMAKE_BINARY_DIR}/samples/python
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy blackscreen.py ${CMAKE_BINARY_DIR}/samples/python/blackscreen.py
)

