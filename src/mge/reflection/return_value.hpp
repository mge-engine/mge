#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/core/type_name.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT return_value
        {

        };

        template <typename T>
        void set_return_value(return_value& rv,
                              T t)
        {
            MGE_THROW(mge::not_yet_implemented(),
                      "Cannot store result type ",
                      mge::type_name<T>());

        }
    }
}
