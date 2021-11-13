#pragma once
#include "mge/script/dllexport.hpp"

namespace mge::script {

    MGESCRIPT_EXPORT void bind_all();
    MGESCRIPT_EXPORT void bind_globals();
    MGESCRIPT_EXPORT void bind_std();

}