// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_preprocessor.hpp"

#include "mge/script/dllexport.hpp"
#include "mge/script/invoke_context.hpp"

#include <type_traits>

namespace mge::script {

    /**
     * @brief Proxy class for implementing interfaces by script methods.
     *
     * @tparam T implemented interface
     */
    template <typename T> class proxy : public T
    {
    public:
        using T::T;
        virtual ~proxy() = default;

        void set_invoke_context(invoke_context* context)
        {
            m_invoke_context = context;
        }

    private:
        invoke_context* m_invoke_context;
    };

#define MGESCRIPT_PROXY_SIGNATURE_ELEMENT_HELPER(z, count, signature)          \
    BOOST_PP_TUPLE_ELEM(count, signature) __p##count

#define MGESCRIPT_PROXY(result_type, method_name, signature, constness)        \
    result_type method_name BOOST_PP_IF(                                       \
        BOOST_PP_TUPLE_SIZE(signature),                                        \
        (BOOST_PP_ENUM(BOOST_PP_TUPLE_SIZE(signature),                         \
                       MGESCRIPT_PROXY_SIGNATURE_ELEMENT_HELPER,               \
                       signature)),                                            \
        ()) BOOST_PP_REMOVE_PARENS(constness) override                         \
    {                                                                          \
        if constexpr (BOOST_PP_TUPLE_SIZE(signature)) {                        \
            return result_type();                                              \
        } else {                                                               \
            return result_type();                                              \
        }                                                                      \
    }

} // namespace mge::script