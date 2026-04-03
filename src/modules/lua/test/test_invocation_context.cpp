// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua.hpp"
#include "lua_invocation_context.hpp"
#include "test/googletest.hpp"

namespace mge {
    MGE_DEFINE_TRACE(LUA);
}

namespace mge::lua {

    class test_invocation_context : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            m_L = luaL_newstate();
            luaL_openlibs(m_L);
        }

        void TearDown() override
        {
            if (m_L) {
                lua_close(m_L);
                m_L = nullptr;
            }
        }

        struct context_setup
        {
            int                                     class_ref;
            int                                     self_ref;
            std::unique_ptr<lua_invocation_context> ctx;
        };

        context_setup make_context(const char* lua_code = nullptr)
        {
            // Create class table
            lua_newtable(m_L);
            if (lua_code) {
                lua_setglobal(m_L, "__test_class__");
                int rc = luaL_dostring(m_L, lua_code);
                EXPECT_EQ(0, rc) << lua_tostring(m_L, -1);
                if (rc != 0) {
                    lua_pop(m_L, 1);
                }
                lua_getglobal(m_L, "__test_class__");
            }
            int class_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);

            // Create self table
            lua_newtable(m_L);
            int self_ref = luaL_ref(m_L, LUA_REGISTRYINDEX);

            auto ctx = std::make_unique<lua_invocation_context>(m_L,
                                                                class_ref,
                                                                self_ref);

            return {class_ref, self_ref, std::move(ctx)};
        }

        lua_State* m_L = nullptr;
    };

    TEST_F(test_invocation_context, call_implemented_returns_false_for_missing)
    {
        auto [cr, sr, ctx] = make_context();
        EXPECT_FALSE(ctx->call_implemented("nonexistent"));
    }

    TEST_F(test_invocation_context, call_implemented_returns_true_for_existing)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.greet = function(self) end");
        EXPECT_TRUE(ctx->call_implemented("greet"));
    }

    TEST_F(test_invocation_context,
           call_implemented_returns_false_for_non_function)
    {
        auto [cr, sr, ctx] = make_context("__test_class__.value = 42");
        EXPECT_FALSE(ctx->call_implemented("value"));
    }

    TEST_F(test_invocation_context, call_void_method)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.setup = function(self)\n"
                         "  __test_called__ = true\n"
                         "end");
        ctx->call<void>("setup");
        lua_getglobal(m_L, "__test_called__");
        EXPECT_TRUE(lua_toboolean(m_L, -1));
        lua_pop(m_L, 1);
    }

    TEST_F(test_invocation_context, call_method_returns_bool)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.check = function(self)\n"
                         "  return true\n"
                         "end");
        bool result = ctx->call<bool>("check");
        EXPECT_TRUE(result);
    }

    TEST_F(test_invocation_context, call_method_returns_false)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.check = function(self)\n"
                         "  return false\n"
                         "end");
        bool result = ctx->call<bool>("check");
        EXPECT_FALSE(result);
    }

    TEST_F(test_invocation_context, call_method_returns_int32)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.compute = function(self)\n"
                         "  return 42\n"
                         "end");
        int32_t result = ctx->call<int32_t>("compute");
        EXPECT_EQ(42, result);
    }

    TEST_F(test_invocation_context, call_method_returns_double)
    {
        auto [cr, sr, ctx] = make_context("__test_class__.pi = function(self)\n"
                                          "  return 3.14\n"
                                          "end");
        double result = ctx->call<double>("pi");
        EXPECT_DOUBLE_EQ(3.14, result);
    }

    TEST_F(test_invocation_context, call_method_returns_string)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.name = function(self)\n"
                         "  return 'hello'\n"
                         "end");
        std::string result = ctx->call<std::string>("name");
        EXPECT_EQ("hello", result);
    }

    TEST_F(test_invocation_context, call_with_int_argument)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.add = function(self, x)\n"
                         "  __test_result__ = x + 1\n"
                         "end");
        ctx->call<void, int32_t>("add", int32_t(10));
        lua_getglobal(m_L, "__test_result__");
        EXPECT_EQ(11, lua_tointeger(m_L, -1));
        lua_pop(m_L, 1);
    }

    TEST_F(test_invocation_context, call_with_string_argument)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.greet = function(self, msg)\n"
                         "  __test_result__ = 'hello ' .. msg\n"
                         "end");
        ctx->call<void, std::string>("greet", std::string("world"));
        lua_getglobal(m_L, "__test_result__");
        EXPECT_STREQ("hello world", lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
    }

    TEST_F(test_invocation_context, call_with_bool_argument)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.toggle = function(self, flag)\n"
                         "  if flag then\n"
                         "    __test_result__ = 'on'\n"
                         "  else\n"
                         "    __test_result__ = 'off'\n"
                         "  end\n"
                         "end");
        ctx->call<void, bool>("toggle", true);
        lua_getglobal(m_L, "__test_result__");
        EXPECT_STREQ("on", lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
    }

    TEST_F(test_invocation_context, call_with_multiple_arguments)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.sum = function(self, a, b)\n"
                         "  return a + b\n"
                         "end");
        int32_t result =
            ctx->call<int32_t, int32_t, int32_t>("sum", int32_t(3), int32_t(4));
        EXPECT_EQ(7, result);
    }

    TEST_F(test_invocation_context, call_not_found_returns_default)
    {
        auto [cr, sr, ctx] = make_context();
        // Calling a non-existent method with void return should not crash
        ctx->call<void>("missing");
    }

    TEST_F(test_invocation_context, call_void_returns_nil_gives_false)
    {
        auto [cr, sr, ctx] =
            make_context("__test_class__.noop = function(self)\n"
                         "  -- returns nil implicitly\n"
                         "end");
        bool result = ctx->call<bool>("noop");
        EXPECT_FALSE(result);
    }

} // namespace mge::lua
