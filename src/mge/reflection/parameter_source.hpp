#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/core/type_name.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT parameter_source
        {
        public:
            parameter_source() = default;
            virtual ~parameter_source() = default;

        };

        template <typename T>
        inline T parameter(parameter_source& source)
        {
            MGE_THROW(mge::not_yet_implemented(),
                      "Cannot retrieve parameter of type ",
                      mge::type_name<T>());
        }
    }
}
