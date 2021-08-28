SET(mgeasset_sources
    asset_type.cpp
)

SET(mgeasset_headers
    dllexport.hpp
    asset_type.hpp
)

MGE_LIBRARY(
    TARGET  mgeasset
    TYPE    SHARED
    SOURCES
    ${mgeasset_sources}
    ${mgeasset_headers}
)

TARGET_LINK_LIBRARIES(mgeasset mgecore boost-all)

#ADD_SUBDIRECTORY(test)
