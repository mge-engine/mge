// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_preprocessor.hpp"

#include "mge/script/dllexport.hpp"
#include "mge/script/invoke_context.hpp"
#include "mge/script/signature.hpp"

#include <tuple>
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

    protected:
        invoke_context* m_invoke_context;
    };

#define MGESCRIPT_PROXY_PARAMETER_WITH_TYPE(z, count, sig)                     \
    BOOST_PP_TUPLE_ELEM(count, sig) __p##count

#define MGESCRIPT_PROXY_PARAMETER_LIST_ELEMENT(z, count, sig) __p##count

#define MGESCRIPT_PROXY(result_type, method_name, sig, constness)              \
    result_type method_name BOOST_PP_IF(                                       \
        BOOST_PP_TUPLE_SIZE(sig),                                              \
        (BOOST_PP_ENUM(BOOST_PP_TUPLE_SIZE(sig),                               \
                       MGESCRIPT_PROXY_PARAMETER_WITH_TYPE,                    \
                       sig)),                                                  \
        ()) BOOST_PP_REMOVE_PARENS(constness) override                         \
    {                                                                          \
        if constexpr (BOOST_PP_TUPLE_SIZE(sig)) {                              \
            static ::mge::script::signature s =                                \
                ::mge::script::signature::create<BOOST_PP_REMOVE_PARENS(       \
                    sig)>();                                                   \
                                                                               \
            return m_invoke_context->invoke<result_type>(                      \
                #method_name,                                                  \
                s,                                                             \
                std::forward_as_tuple(                                         \
                    BOOST_PP_ENUM(BOOST_PP_TUPLE_SIZE(sig),                    \
                                  MGESCRIPT_PROXY_PARAMETER_LIST_ELEMENT,      \
                                  sig)));                                      \
        } else {                                                               \
            return m_invoke_context->invoke<result_type>(#method_name);        \
        }                                                                      \
    }

} // namespace mge::script