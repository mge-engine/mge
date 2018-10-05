# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

CONFIGURE_FILE (
        "mge.doxyconfig.in"
        "mge.doxyconfig"
)

ADD_CUSTOM_TARGET(apidoc
        COMMAND ${DOXYGEN_EXECUTABLE} mge.doxyconfig
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc/api
        COMMENT "Create API documentation using doxygen"
        VERBATIM)
