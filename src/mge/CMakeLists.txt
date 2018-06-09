
CONFIGURE_FILE (
        "version.hpp.in"
        "version.hpp"
)

CONFIGURE_FILE (
        "config.hpp.in"
        "config.hpp"
)

ADD_SUBDIRECTORY(core)
