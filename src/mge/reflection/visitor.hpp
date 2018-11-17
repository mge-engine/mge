#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include <string>

namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT visitor
        {
        public:
            visitor() = default;
            virtual ~visitor() = default;

            virtual void start(module& m);
            virtual void finish(module& m);
            virtual void start(type& t);
            virtual void finish(type& t);
            virtual void enum_value(const std::string& name,
                                    int64_t value);
        };

    }
}
