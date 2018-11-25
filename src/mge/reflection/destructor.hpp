#pragma once
#include "mge/reflection/dllexport.hpp"
#include <functional>
namespace mge {
    namespace reflection {
        class MGE_REFLECTION_EXPORT destructor
        {
        public:
            typedef std::function<void (void *)> function;

            destructor() = default;
            destructor(const function& f)
                :m_function(f)
            {}

            void invoke(void *);

        private:
            function m_function;
        };
    }
}
