# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(BOOST_ROOT "${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/install")
FIND_PACKAGE(Boost COMPONENTS filesystem)
IF(NOT "${Boost_FOUND}")
    MESSAGE("-- Did not find boost in the system, look for previous build location")
    IF(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/src/boost-stamp/boost-install")
        MESSAGE("-- Need to download and install boost, not found on the system")
        FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/boost)
        SET(CMAKE_LIST_CONTENT "
        CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
        INCLUDE(ExternalProject)

        EXTERNALPROJECT_ADD(boost
                            PREFIX boost
                            URL https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.zip
                            URL_HASH SHA256=3b1db0b67079266c40b98329d85916e910bbadfc3db3e860c049056788d4d5cd
                            BUILD_IN_SOURCE TRUE
                            CONFIGURE_COMMAND \"bootstrap.bat\"
                            BUILD_COMMAND \"\"
                            INSTALL_COMMAND \"b2\"  \"--prefix=\${PROJECT_SOURCE_DIR}\\\\boost\\\\install\" \"--layout=system\" \"--without-python\" \"toolset=msvc\" \"variant=release\" \"link=shared\" \"threading=multi\" \"runtime-link=shared\" \"address-model=64\" \"install\"
        )
        ")
        FILE(WRITE ${CMAKE_BINARY_DIR}/external/boost/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/boost
        )
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
                         WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/boost
        )
    ENDIF()
    SET(BOOST_ROOT "${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/install")
    FIND_PACKAGE(Boost COMPONENTS filesystem)
    IF(NOT "${Boost_FOUND}")
        MESSAGE(FATAL "Boost could not be found and internal build process failed")
    ENDIF()
ENDIF()

FILE(GLOB all_boost_dlls "${Boost_LIBRARY_DIR_RELEASE}/*.dll")
FOREACH(boost_dll ${all_boost_dlls})
    FILE(COPY ${boost_dll} DESTINATION ${CMAKE_BINARY_DIR})
ENDFOREACH()
ADD_LIBRARY(boost-all INTERFACE IMPORTED GLOBAL)
SET_PROPERTY(TARGET boost-all
              PROPERTY INTERFACE_LINK_LIBRARIES Boost::boost Boost::disable_autolinking Boost::dynamic_linking Boost::filesystem)

