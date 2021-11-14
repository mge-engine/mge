#include "mge/script/bind.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"
#include <mutex>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}
namespace mge::script {

    void bind_globals()
    {
        module()(type<void>(),
                 type<char>(),
                 type<unsigned char>(),
                 type<signed char>(),
                 type<short>(),
                 type<unsigned short>());
    }

    void bind_std() { module("std")(type<std::string>()); }

    std::once_flag s_bind_all_complete;

    void bind_all()
    {
        std::call_once(s_bind_all_complete, [] {
            try {
                mge::script::bind_globals();
                mge::script::bind_std();
            } catch (const mge::exception& e) {
                MGE_ERROR_TRACE(SCRIPT) << "Binding global types failed: " << e;
                throw;
            }
        });
    }
} // namespace mge::script