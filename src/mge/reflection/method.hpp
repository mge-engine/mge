#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/parameter_source.hpp"
#include "mge/reflection/return_value.hpp"
#include <functional>
namespace mge {
    namespace reflection {

        class MGE_REFLECTION_EXPORT method
        {
        public:
            typedef std::function<void (return_value&,
                                        void *,
                                        parameter_source&)> function;
            method(const char *name,
                   const method::function& f);
            ~method() = default;
            void invoke(return_value& result,
                        void *this_ptr,
                        parameter_source& parameters);
        private:
            std::string m_name;
            function    m_function;
        };

    }
}
