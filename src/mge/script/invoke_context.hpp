// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/call_debugger.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/signature.hpp"

#include <iostream>
#include <tuple>

namespace mge::script {

    /**
     * @brief Invocation context.
     */
    class MGESCRIPT_EXPORT invoke_context
    {
    public:
        invoke_context() = default;
        virtual ~invoke_context() = default;

        template <typename R> R invoke(const char* name)
        {
            call(name);
            return retrieve_result<R>();
        }

        template <typename R, typename... Args>
        R invoke(const char*                   name,
                 const mge::script::signature& s,
                 const std::tuple<Args...>&    args)
        {
            return R();
        }

        template <typename R> struct result_retriever
        {
            result_retriever(invoke_context* context) {}

            R retrieve() const
            {
                call_debugger();
                return R();
            }
        };

        template <> struct result_retriever<void>
        {
            result_retriever(invoke_context* context) {}

            void retrieve() const {}
        };

#define SIMPLE_RETRIEVER(type)                                                 \
    template <> struct result_retriever<type>                                  \
    {                                                                          \
        result_retriever(invoke_context* context)                              \
            : m_context(context)                                               \
        {}                                                                     \
        type retrieve() const { return m_context->retrieve_##type(); }         \
        invoke_context* m_context;                                             \
    }

        SIMPLE_RETRIEVER(bool);
        SIMPLE_RETRIEVER(int8_t);
        SIMPLE_RETRIEVER(int16_t);
        SIMPLE_RETRIEVER(int32_t);
        SIMPLE_RETRIEVER(int64_t);
        SIMPLE_RETRIEVER(uint8_t);
        SIMPLE_RETRIEVER(uint16_t);
        SIMPLE_RETRIEVER(uint32_t);
        SIMPLE_RETRIEVER(uint64_t);
        SIMPLE_RETRIEVER(float);
        SIMPLE_RETRIEVER(double);

#undef SIMPLE_RETRIEVER
        template <typename T> T retrieve_result()
        {
            result_retriever<T> r(this);
            std::cerr << "Retrieve " << typeid(T).name() << std::endl;
            return r.retrieve();
        }

        virtual void call(const char* method) = 0;

        virtual bool     retrieve_bool() const = 0;
        virtual int8_t   retrieve_int8_t() const = 0;
        virtual int16_t  retrieve_int16_t() const = 0;
        virtual int32_t  retrieve_int32_t() const = 0;
        virtual int64_t  retrieve_int64_t() const = 0;
        virtual uint8_t  retrieve_uint8_t() const = 0;
        virtual uint16_t retrieve_uint16_t() const = 0;
        virtual uint32_t retrieve_uint32_t() const = 0;
        virtual uint64_t retrieve_uint64_t() const = 0;
        virtual float    retrieve_float() const = 0;
        virtual double   retrieve_double() const = 0;
    };

} // namespace mge::script