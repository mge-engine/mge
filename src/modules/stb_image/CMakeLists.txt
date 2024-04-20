SET(mge_module_stb_image_SOURCES
    stb_image.cpp
)

ADD_LIBRARY(mge_module_stb_image SHARED ${mge_module_stb_image_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_stb_image mgecore mgeasset mgegraphics)