// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "lua_context.hpp"
#include "lua_error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"

MGE_DEFINE_LOG(LUA);

#include <cfloat>
#include <limits>

#ifdef min
#    undef min
#endif
#ifdef max
#    undef max
#endif

namespace lua {
    lua_context::lua_context() : m_lua_state(nullptr)
    {
        m_lua_state = luaL_newstate();
        luaL_openlibs(m_lua_state);
    }

    lua_context::~lua_context()
    {
        if (m_lua_state) {
            lua_close(m_lua_state);
        }
    }

    void lua_context::eval(const char *script)
    {
        if (!script) {
            MGE_THROW_ARGUMENT_NOT_NULL(script);
        }
        size_t script_len = strlen(script);
        int    rc = luaL_loadbuffer(m_lua_state, script, script_len, "");
        CHECK_STATUS(rc, m_lua_state);
        rc = lua_pcall(m_lua_state, 0, 0, 0);
        CHECK_STATUS(rc, m_lua_state);
    }

    template <typename I>
    static I global_variable(lua_State *L, const char *name, const char *type)
    {
        if (name == nullptr) {
            MGE_THROW_ARGUMENT_NOT_NULL(name);
        } else {
            int ty = lua_getglobal(L, name);
            if (ty == LUA_TNUMBER || ty == LUA_TSTRING) {
                int        isnum = 0;
                lua_Number n     = lua_tonumberx(L, 1, &isnum);
                if (isnum) {
                    lua_pop(L, 1);
                    if (n < std::numeric_limits<I>::min() ||
                        n > std::numeric_limits<I>::max()) {
                        MGE_THROW(mge::runtime_exception)
                            << "Cannot convert value to " << type
                            << ": numeric overflow";
                    }
                    return (I)n;
                } else {
                    lua_pop(L, 1);
                    MGE_THROW(mge::bad_cast)
                        << "Cannot convert value to " << type;
                }
            } else {
                lua_pop(L, 1);
                MGE_THROW(mge::bad_cast)
                    << "Cannot convert value of type " << lua_typename(L, ty)
                    << " to " << type;
            }
        }
    }

    signed char lua_context::signed_char_value(const char *name)
    {
        return global_variable<signed char>(m_lua_state, name, "signed char");
    }

    char lua_context::char_value(const char *name)
    {
        return global_variable<char>(m_lua_state, name, "char");
    }

    unsigned char lua_context::unsigned_char_value(const char *name)
    {
        return global_variable<unsigned char>(m_lua_state, name,
                                              "unsigned char");
    }

    short lua_context::short_value(const char *name)
    {
        return global_variable<short>(m_lua_state, name, "short");
    }

    unsigned short lua_context::unsigned_short_value(const char *name)
    {
        return global_variable<unsigned short>(m_lua_state, name,
                                               "unsigned short");
    }

    int lua_context::int_value(const char *name)
    {
        return global_variable<int>(m_lua_state, name, "int");
    }

    unsigned int lua_context::unsigned_int_value(const char *name)
    {
        return global_variable<unsigned int>(m_lua_state, name, "unsigned int");
    }

    long lua_context::long_value(const char *name)
    {
        return global_variable<long>(m_lua_state, name, "long");
    }

    unsigned long lua_context::unsigned_long_value(const char *name)
    {
        return global_variable<unsigned long>(m_lua_state, name,
                                              "unsigned long");
    }
    long long lua_context::long_long_value(const char *name)
    {
        return global_variable<long long>(m_lua_state, name, "long long");
    }

    unsigned long long lua_context::unsigned_long_long_value(const char *name)
    {
        return global_variable<unsigned long long>(m_lua_state, name,
                                                   "unsigned long long");
    }

    std::string lua_context::string_value(const char *name)
    {
        if (name == nullptr) {
            MGE_THROW_ARGUMENT_NOT_NULL(name);
        } else {
            int ty = lua_getglobal(m_lua_state, name);
            if (ty == LUA_TNUMBER || ty == LUA_TSTRING || ty == LUA_TBOOLEAN) {
                size_t      len = 0;
                const char *str = lua_tolstring(m_lua_state, 1, &len);
                std::string result(str, str + len);
                lua_pop(m_lua_state, 1);
                return result;
            } else {
                lua_pop(m_lua_state, 1);
                MGE_THROW(mge::bad_cast)
                    << "Cannot convert value of type "
                    << lua_typename(m_lua_state, ty) << " to string";
            }
        }
    }

    bool lua_context::bool_value(const char *name)
    {
        if (name == nullptr) {
            MGE_THROW_ARGUMENT_NOT_NULL(name);
        } else {
            lua_getglobal(m_lua_state, name);
            int val = lua_toboolean(m_lua_state, 1);
            lua_pop(m_lua_state, 1);
            return val != 0;
        }
    }

    float lua_context::float_value(const char *name)
    {
        return global_variable<float>(m_lua_state, name, "float");
    }

    double lua_context::double_value(const char *name)
    {
        return global_variable<double>(m_lua_state, name, "double");
    }

    long double lua_context::long_double_value(const char *name)
    {
        return global_variable<long double>(m_lua_state, name, "long double");
    }

} // namespace lua
