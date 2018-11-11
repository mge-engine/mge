SET(mge_opengl_module_SOURCES
    render_system.cpp)
IF(WIN32)
    LIST(APPEND mge_opengl_module_SOURCES
         ${CMAKE_SOURCE_DIR}/src/win32/monitor.cpp
#         ${CMAKE_SOURCE_DIR}/src/shared/win32_window.cpp
    )
ENDIF()
ADD_LIBRARY(mge_module_opengl MODULE
            ${mge_opengl_module_SOURCES})
TARGET_INCLUDE_DIRECTORIES(mge_module_opengl PRIVATE ${OPENGL_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(mge_module_opengl mgecore mgegraphics gl3w ${OPENGL_LIBRARIES})
