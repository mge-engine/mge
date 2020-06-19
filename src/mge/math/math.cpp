#include "mge/core/artifact.hpp"
#include "mge/math/glm.hpp"

MGE_ARTIFACT(glm, "G-Truc Creation", "https://glm.g-truc.net/0.9.9/index.html",
             mge::version(GLM_VERSION_MAJOR, GLM_VERSION_MINOR,
                          GLM_VERSION_PATCH),
             mge::license(mge::license_type::MIT))
    ->used_by("mge");