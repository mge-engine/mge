#pragma once
#include "mge/script/dllexport.hpp"
#include <type_traits>

namespace mge::script {
    /**
     * @brief A call context is used when invoking functions or methods.
     *
     */
    class MGESCRIPT_EXPORT call_context
    {
    public:
        call_context() = default;
        virtual ~call_context() = default;

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

        /**
         * @brief Invoked when an integer result is stored.
         * 
         * @param result call result to be stored.
         */
        virtual void store_result(int result) = 0;

    protected:
        virtual bool bool_parameter(size_t position) = 0;
        virtual int  int_parameter(size_t position) = 0;
    };
} // namespace mge::script