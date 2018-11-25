#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/parameter_source.hpp"
#include <functional>
namespace mge {
    namespace reflection{

        class MGE_REFLECTION_EXPORT constructor
        {
        public:
            typedef std::function<void (void*, const parameter_source&)> function;

            constructor() = default;

            constructor(const signature& s,
                        const function& f)
                :m_parameter_types(s)
                ,m_function(f)
            {}

            constructor(const constructor& c)
                :m_parameter_types(c.m_parameter_types)
                ,m_function(c.m_function)
            {}

            constructor(constructor&& c)
                :m_parameter_types(std::move(c.m_parameter_types))
                ,m_function(std::move(c.m_function))
            {}

            ~constructor() = default;

            constructor& operator =(const constructor& c)
            {
                m_parameter_types = c.m_parameter_types;
                m_function = c.m_function;
                return *this;
            }

            constructor& operator =(constructor&& c)
            {
                m_parameter_types = std::move(c.m_parameter_types);
                m_function = std::move(c.m_function);
                return *this;
            }

            void invoke(void *object, const parameter_source& parameters);

            const signature& parameter_types() const
            {
                return m_parameter_types;
            }

        private:
            signature m_parameter_types;
            function  m_function;
        };


    }
}
