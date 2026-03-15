// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/proxy.hpp"
#include "mge/reflection/type.hpp"
#include "test/googletest.hpp"

#include <string>

namespace mge::reflection {

    namespace {

        class base
        {
        public:
            base() = default;
            base(int v)
                : m_value(v)
            {}
            base(int v, const std::string& s)
                : m_value(v)
                , m_name(s)
            {}
            virtual ~base() = default;

            int value() const
            {
                return m_value;
            }
            const std::string& name() const
            {
                return m_name;
            }

        private:
            int         m_value{0};
            std::string m_name;
        };

        class test_proxy : public proxy<base>
        {
        public:
            using proxy<base>::proxy;
            invocation_context* context() const
            {
                return m_context;
            }
        };

    } // namespace

    TEST(proxy, default_construct)
    {
        test_proxy p;
        EXPECT_EQ(p.value(), 0);
        EXPECT_EQ(p.name(), "");
        EXPECT_EQ(p.context(), nullptr);
    }

    TEST(proxy, forward_single_argument)
    {
        test_proxy p(42);
        EXPECT_EQ(p.value(), 42);
    }

    TEST(proxy, forward_multiple_arguments)
    {
        test_proxy p(7, std::string("hello"));
        EXPECT_EQ(p.value(), 7);
        EXPECT_EQ(p.name(), "hello");
    }

    TEST(proxy, set_context)
    {
        test_proxy p;
        EXPECT_EQ(p.context(), nullptr);
        auto dummy = reinterpret_cast<invocation_context*>(0x1234);
        p.set_context(dummy);
        EXPECT_EQ(p.context(), dummy);
    }

    TEST(proxy, base_type_alias)
    {
        static_assert(std::is_same_v<proxy<base>::base_type, base>);
    }

    TEST(proxy, attach_proxy_type)
    {
        type<base> t;
        t.proxy_type<test_proxy>();

        auto& specific = get_or_create_type_details<base>()->class_specific();
        EXPECT_NE(specific.proxy_type, nullptr);
        EXPECT_EQ(specific.proxy_type,
                  get_or_create_type_details<test_proxy>());

        auto& proxy_specific =
            get_or_create_type_details<test_proxy>()->class_specific();
        EXPECT_EQ(proxy_specific.interface_type,
                  get_or_create_type_details<base>());
    }

} // namespace mge::reflection
