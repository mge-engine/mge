# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
FUNCTION(COPY_FILE SOURCE TARGET)
    SET(__target_name "copy-${SOURCE}-to-${TARGET}")
    STRING(REPLACE "/" "_" __target_name "${__target_name}")
    STRING(REPLACE ":" "_" __target_name "${__target_name}")
    STRING(REPLACE "." "_" __target_name "${__target_name}")
    STRING(REPLACE " " "_" __target_name "${__target_name}")
    ADD_CUSTOM_TARGET("${__target_name}"
                      ALL
                      DEPENDS ${TARGET})
    CMAKE_PATH(GET TARGET PARENT_PATH _target_directory)
    ADD_CUSTOM_COMMAND(OUTPUT ${TARGET}
                       COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE} ${TARGET}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       DEPENDS ${SOURCE}
                               ${__target_directory})
ENDFUNCTION()
