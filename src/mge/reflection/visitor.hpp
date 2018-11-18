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

            virtual void start(const module& m);
            virtual void finish(const module& m);
            virtual void start(const type& t);
            virtual void finish(const type& t);
            virtual void enum_value(const std::string& name,
                                    int64_t value);
        };

    }
}
