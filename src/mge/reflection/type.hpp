#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"

namespace mge {
    namespace reflection {

        template <typename T> class type_wrapper;

        class MGE_REFLECTION_EXPORT class type
        {
        public:
            template <typename T>
            static type_wrapper<T> get();
        private:
            identifier m_type_id;
        };



        template <typename T>
        class type_wrapper
        {
        public:
            typedef T wrapped_type;
        private:
            type m_type;
        }
    }
}

namespace mge {
    namespace reflection {

        template <typename T>
        type_wrapper<T> type::get()
        {

        }

    }
}
