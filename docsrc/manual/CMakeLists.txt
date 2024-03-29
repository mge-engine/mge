# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(DOXYGEN_FOUND)
    GET_PROPERTY(DOXYGEN_INPUT_FILE_LIST GLOBAL PROPERTY ALL_PROJECT_SOURCES)
    FOREACH(FILE ${DOXYGEN_INPUT_FILE_LIST})
        SET(DOXYGEN_INPUT_FILES "${DOXYGEN_INPUT_FILES} ${FILE}")
        FILE(TO_CMAKE_PATH __cmake_file "${FILE}")
    ENDFOREACH()
    SET(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
    SET(DOXYGEN_XML_INDEX_FILE "${DOXYGEN_OUTPUT_DIRECTORY}/xml/index.xml")
    CONFIGURE_FILE(Doxyfile.in Doxyfile)
    FILE(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
    ADD_CUSTOM_TARGET(doxygen-docs
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                      BYPRODUCTS ${DOXYGEN_XML_INDEX_FILE}
                      COMMENT "Generate doxygen documentation"
                      DEPENDS ${DOXYGEN_INPUT_FILE_LIST})
    IF(SPHINX_FOUND)
        SET(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
        SET(SPHINX_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/manual-html)
        SET(SPHINX_OUTPUT_PDF ${CMAKE_CURRENT_BINARY_DIR}/manual-pdf)
        FILE(MAKE_DIRECTORY ${SPHINX_OUTPUT})
        ADD_CUSTOM_TARGET(manual-html
            DEPENDS ${DOXYGEN_XML_INDEX_FILE}
            COMMAND ${SPHINX_EXECUTABLE}
                -b html
                -Dbreathe_projects.mge=${DOXYGEN_OUTPUT_DIRECTORY}/xml
                ${SPHINX_SOURCE} ${SPHINX_OUTPUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generate html manual"
        )
        ADD_DEPENDENCIES(documentation manual-html)
        ADD_CUSTOM_TARGET(manual-latex
            DEPENDS ${DOXYGEN_XML_INDEX_FILE}
            COMMAND ${SPHINX_EXECUTABLE}
                -b latex
                -Dbreathe_projects.mge=${DOXYGEN_OUTPUT_DIRECTORY}/xml
                ${SPHINX_SOURCE} ${SPHINX_OUTPUT_PDF}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generate html manual"
        )
    ELSE()
        MESSAGE("-- Sphinx not found, not building manual")
    ENDIF()


ELSE()
    MESSAGE("-- Doxygen was not found, not building documentation")
ENDIF()
