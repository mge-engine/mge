// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_preprocessor.hpp"

#include "mge/core/nargs.hpp"
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

#define MGESCRIPT_PROXY_METHOD(result_type, method_name, sig, constness)       \
    result_type method_name BOOST_PP_IF(                                       \
        MGE_IS_EMPTY(BOOST_PP_REMOVE_PARENS(sig)),                             \
        sig,                                                                   \
        (BOOST_PP_ENUM(BOOST_PP_TUPLE_SIZE(sig),                               \
                       MGESCRIPT_PROXY_PARAMETER_WITH_TYPE,                    \
                       sig))) BOOST_PP_REMOVE_PARENS(constness) override       \
        MGESCRIPT_PROXY_METHOD_BLOCK(result_type, method_name, sig)

#define MGESCRIPT_PROXY_METHOD_BLOCK(result_type, method_name, sig)            \
    BOOST_PP_IF(MGE_IS_EMPTY(BOOST_PP_REMOVE_PARENS(sig)),                     \
                MGESCRIPT_PROXY_METHOD_BLOCK_EMPTY_SIGNATURE(result_type,      \
                                                             method_name),     \
                MGESCRIPT_PROXY_METHOD_BLOCK_WITH_SIGNATURE(result_type,       \
                                                            method_name,       \
                                                            sig))

#define MGESCRIPT_PROXY_METHOD_BLOCK_EMPTY_SIGNATURE(result_type, method_name) \
    {                                                                          \
        return m_invoke_context->invoke<result_type>(#method_name);            \
    }

#define MGESCRIPT_PROXY_METHOD_BLOCK_WITH_SIGNATURE(result_type,               \
                                                    method_name,               \
                                                    sig)                       \
    {                                                                          \
        static ::mge::script::signature s =                                    \
            ::mge::script::signature::create<BOOST_PP_REMOVE_PARENS(sig)>();   \
                                                                               \
        return m_invoke_context->invoke<result_type>(                          \
            #method_name,                                                      \
            s,                                                                 \
            std::forward_as_tuple(                                             \
                BOOST_PP_ENUM(BOOST_PP_TUPLE_SIZE(sig),                        \
                              MGESCRIPT_PROXY_PARAMETER_LIST_ELEMENT,          \
                              sig)));                                          \
    }

} // namespace mge::script
