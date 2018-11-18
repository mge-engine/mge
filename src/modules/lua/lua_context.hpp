#pragma once
#include "lua.hpp"
#include "mge/script/script_context.hpp"

namespace lua {
    class lua_context : public mge::script_context
    {
    public:
        lua_context();
        virtual ~lua_context();
        void bind(const mge::reflection::module& module) override;
        void eval(const char *script) override;

        signed char signed_char_value(const char *name) override;
        char char_value(const char *name) override;
        unsigned char unsigned_char_value(const char *name) override;
        short short_value(const char *name) override;
        unsigned short unsigned_short_value(const char *name) override;
        int int_value(const char *name) override;
        unsigned int unsigned_int_value(const char *name) override;
        long long_value(const char *name) override;
        unsigned long unsigned_long_value(const char *name) override;
        long long long_long_value(const char *name) override;
        unsigned long long unsigned_long_long_value(const char *name) override;
        std::string string_value(const char *name) override;
        bool bool_value(const char *name) override;
        float float_value(const char *name) override;
        double double_value(const char *name) override;
        long double long_double_value(const char *name) override;

        lua_State *state() const { return m_lua_state; }
    private:
        lua_State *m_lua_state;
    };
}
