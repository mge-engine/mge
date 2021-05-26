FUNCTION(COPY_FILE SOURCE TARGET)
    SET(__target_name "copy-${SOURCE}-to-${TARGET}")
    STRING(REPLACE "/" "_" __target_name "${__target_name}")
    STRING(REPLACE ":" "_" __target_name "${__target_name}")
    STRING(REPLACE "." "_" __target_name "${__target_name}")
    STRING(REPLACE " " "_" __target_name "${__target_name}")
    ADD_CUSTOM_TARGET("${__target_name}"
                      ALL
                      DEPENDS ${TARGET})
    ADD_CUSTOM_COMMAND(OUTPUT ${TARGET}
                       COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE} ${TARGET}
                       DEPENDS ${SOURCE})
ENDFUNCTION()
