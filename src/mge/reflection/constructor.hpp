#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/parameter_source.hpp"
#include <functional>
namespace mge {
    namespace reflection{

        class constructor
        {
        public:
            typedef std::function<void (void*, const parameter_source&)> constructor_function;

            constructor(const signature& s,
                        const constructor_function& f)
                :m_parameter_types(s)
                ,m_function(f)
            {}

            ~constructor() = default;

            void invoke(void *object, const parameter_source& parameters);

            const signature& parameter_types() const
            {
                return m_parameter_types;
            }

        private:
            signature m_parameter_types;
            constructor_function m_function;
        };


    }
}
