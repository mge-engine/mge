# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_CUSTOM_TARGET(fixcopyright
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/fixcopyright.py"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

ADD_CUSTOM_TARGET(projectfiles
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/projectfiles.py" >moge.files
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

ADD_CUSTOM_TARGET(coverage-report
  DEPENDS coverage-tests-run
  COMMAND ${DRCOV2LCOV} -dir ${CMAKE_BINARY_DIR}/coverage
  COMMAND  ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/coverage-report.py" ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src/external ${CMAKE_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
