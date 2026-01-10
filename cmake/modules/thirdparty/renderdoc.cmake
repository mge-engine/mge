# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(NOT HEADLESS_ENVIRONMENT)
    IF(WIN32)
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/renderdoc)
    SET(CMAKE_LIST_CONTENT "
        CMAKE_MINIMUM_REQUIRED(VERSION 3.27)
        PROJECT(RENDERDOC)
        INCLUDE(ExternalProject)
        EXTERNALPROJECT_ADD(renderdoc-external
                            PREFIX renderdoc
                            GIT_REPOSITORY \"https://github.com/mge-engine/renderdoc.git\"
                            GIT_TAG \"mge-local\"
                            INSTALL_COMMAND \"\"
                            BUILD_COMMAND msbuild renderdoc.sln /m /p:PlatformToolset=v143 /p:Configuration=Release
                            CONFIGURE_COMMAND \"\"
                            BUILD_IN_SOURCE TRUE

        )
    ")
    FILE(WRITE ${CMAKE_BINARY_DIR}/external/renderdoc/CMakeLists.txt "${CMAKE_LIST_CONTENT}")  
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                    WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/renderdoc
    )
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
                    WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/renderdoc
    )
    ENDIF()  
ENDIF()