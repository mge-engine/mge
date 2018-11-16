#include "lua_context.hpp"
#include "mge/reflection/module.hpp"

namespace lua {
    lua_context::lua_context()
        :m_lua_state(nullptr)
    {
        m_lua_state = luaL_newstate();
        luaL_openlibs(m_lua_state);
    }

    lua_context::~lua_context()
    {
        if(m_lua_state) {
            lua_close(m_lua_state);
        }
    }

    void
    lua_context::bind(mge::reflection::module m)
    {}

    void
    lua_context::eval(const char *script)
    {}

#if 0
    template <typename I>
    static I global_variable(lua_State *L, const char *name, const char *type)
    {
        if(name == nullptr) {
            throw MOGE_EXCEPTION(moge::null_pointer) << "Variable name must not be null";
        } else {
            int ty = lua_getglobal(L, name);
            if(ty == LUA_TNUMBER || ty == LUA_TSTRING) {
                int isnum = 0;
                lua_Number n = lua_tonumberx(L, 1, &isnum);
                if(isnum) {
                    lua_pop(L, 1);
                    if(n < std::numeric_limits<I>::min() || n > std::numeric_limits<I>::max()) {
                        throw MOGE_EXCEPTION(moge::numeric_overflow) << "Cannot convert value to " << type << ": numeric overflow";
                    }
                    return (I)n;
                } else {
                    lua_pop(L, 1);
                    throw MOGE_EXCEPTION(moge::bad_cast) << "Cannot convert value to " << type;
                }
            } else {
                lua_pop(L, 1);
                throw MOGE_EXCEPTION(moge::bad_cast) << "Cannot convert value of type " << lua_typename(L, ty) << " to " << type;
            }
        }
    }
#endif

    signed char
    lua_context::signed_char_value(const char *name)
    {
        return 0;
    }

    char
    lua_context::char_value(const char *name)
    {
        return 0;
    }

    unsigned char
    lua_context::unsigned_char_value(const char *name)
    {
        return 0;
    }

    short
    lua_context::short_value(const char *name)
    {
        return 0;
    }

    unsigned short
    lua_context::unsigned_short_value(const char *name)
    {
        return 0;
    }

    int
    lua_context::int_value(const char *name)
    {
        return 0;
    }


    unsigned int
    lua_context::unsigned_int_value(const char *name)
    {
        return 0;
    }

    long
    lua_context::long_value(const char *name)
    {
        return 0;
    }

    unsigned long
    lua_context::unsigned_long_value(const char *name)
    {
        return 0;
    }
    long long
    lua_context::long_long_value(const char *name)
    {
        return 0;
    }

    unsigned long long
    lua_context::unsigned_long_long_value(const char *name)
    {
        return 0;
    }

    std::string
    lua_context::string_value(const char *name)
    {
        return "";
    }

    bool
    lua_context::bool_value(const char *name)
    {
        return false;
    }

    float
    lua_context::float_value(const char *name)
    {
        return 0.0;
    }

    double
    lua_context::double_value(const char *name)
    {
        return 0.0;
    }

    long double
    lua_context::long_double_value(const char *name)
    {
        return 0;
    }

}
