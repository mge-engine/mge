#include "mge/core/crash.hpp"

#include "mge/script/bind.hpp"
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"

namespace mge::script {
    void bind_core() { module("mge")(function("crash", &mge::crash)); }
} // namespace mge::script