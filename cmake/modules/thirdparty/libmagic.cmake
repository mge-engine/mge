# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(unofficial-libmagic REQUIRED)
#     target_link_libraries(main PRIVATE unofficial::libmagic::libmagic)
MESSAGE(STATUS "Copy dictionary ${unofficial-libmagic_DICTIONARY}")
CONFIGURE_FILE(${unofficial-libmagic_DICTIONARY} ${CMAKE_BINARY_DIR}/mge_magic.mgc COPYONLY)