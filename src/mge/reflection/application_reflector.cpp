// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/application/application.hpp"
#include "mge/reflection/invocation_context.hpp"
#include "mge/reflection/proxy.hpp"

namespace mge::reflection {

    namespace {

        class application_proxy : public proxy<mge::application>
        {
        public:
            application_proxy() = default;
            ~application_proxy() override = default;

            void setup() override
            {
                if (m_context && m_context->call_implemented("setup")) {
                    m_context->call<void>("setup");
                } else {
                    mge::application::setup();
                }
            }

            void async_setup() override
            {
                if (m_context && m_context->call_implemented("async_setup")) {
                    m_context->call<void>("async_setup");
                } else {
                    mge::application::async_setup();
                }
            }

            void teardown() override
            {
                if (m_context && m_context->call_implemented("teardown")) {
                    m_context->call<void>("teardown");
                } else {
                    mge::application::teardown();
                }
            }

            void run() override
            {
                if (m_context && m_context->call_implemented("run")) {
                    m_context->call<void>("run");
                } else {
                    mge::application::run();
                }
            }
        };

    } // namespace

    class application_reflector : public reflector
    {
    public:
        application_reflector() = default;
        ~application_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::application>()
                    .static_method("instance", &mge::application::instance)
                    .proxy_type<application_proxy>());
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 2> deps{{"core", "std"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_reflector,
                                mge::reflection::reflector,
                                application);
} // namespace mge::reflection
