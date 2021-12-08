#pragma once
#include "mge/script/dllexport.hpp"
#include <type_traits>

namespace mge::script {
    /**
     * @brief A parameter context is used when invoking functions or methods.
     *
     */
    class MGESCRIPT_EXPORT parameter_context
    {
    public:
        parameter_context() = default;
        virtual ~parameter_context() = default;

        template <typename T>
        typename std::enable_if<std::is_same<T, bool>::value, T>::type
        parameter(size_t position)
        {
            return bool_parameter(position);
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, int>::value, T>::type
        parameter(size_t position)
        {
            return int_parameter(position);
        }

    protected:
        virtual bool bool_parameter(size_t position) = 0;
        virtual int  int_parameter(size_t position) = 0;
    };
} // namespace mge::script