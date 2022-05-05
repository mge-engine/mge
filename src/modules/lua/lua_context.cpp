// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.

#include "lua_context.hpp"
#include "lua_binder.hpp"
#include "lua_error.hpp"
#include "lua_module.hpp"

#include "mge/core/line_editor.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_view_literals;

namespace mge::lua {

    lua_context::lua_context()
        : m_lua_state(nullptr)
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

#if 0
    static void print_usage(const char* progname, const char* badoption)
    {
        lua_writestringerror("%s: ", progname);
        if (badoption[1] == 'e' || badoption[1] == 'l')
            lua_writestringerror("'%s' needs argument\n", badoption);
        else
            lua_writestringerror("unrecognized option '%s'\n", badoption);
        lua_writestringerror(
            "usage: %s [options] [script [args]]\n"
            "Available options are:\n"
            "  -e stat   execute string 'stat'\n"
            "  -i        enter interactive mode after executing 'script'\n"
            "  -l mod    require library 'mod' into global 'mod'\n"
            "  -l g=mod  require library 'mod' into global 'g'\n"
            "  -v        show version information\n"
            "  -E        ignore environment variables\n"
            "  -W        turn warnings on\n"
            "  --        stop handling options\n"
            "  -         stop handling options and execute stdin\n",
            progname);
    }

    static void print_version(void)
    {
        lua_writestring(LUA_COPYRIGHT, strlen(LUA_COPYRIGHT));
        lua_writeline();
    }
#endif

    struct interpreter_options
    {
        interpreter_options()
            : interactive(false)
            , show_version(false)
            , warnings(false)
        {}

        std::string              exec;
        bool                     interactive;
        std::vector<std::string> libraries;
        bool                     show_version;
        bool                     warnings;
        std::string              script;
        std::vector<std::string> script_args;
    };

    static void print_usage() {}

    static void print_version() {}

    static void
    parse_options(int argc, const char** argv, interpreter_options& options)
    {}

    int lua_context::main(int argc, const char** argv)
    {
        interpreter_options options;

        try {
            parse_options(argc, argv, options);
        } catch (const mge::exception& ex) {
            std::cerr << ex.what() << std::endl;
            print_usage();
            return 1;
        }

        if (options.show_version) {
            print_version();
            return 0;
        }

        return 0;
    }

    void lua_context::eval(const std::string& code)
    {
        int rc = luaL_loadbuffer(m_lua_state, code.data(), code.size(), "");
        CHECK_STATUS(rc, m_lua_state);
        rc = lua_pcall(m_lua_state, 0, 0, 0);
        CHECK_STATUS(rc, m_lua_state);
    }

    void lua_context::bind(const mge::script::module& m)
    {
        lua_binder b(*this);
        b.bind(m);
    }

    lua_module_ref lua_context::get_or_add_module(const mge::script::module& m)
    {
        auto it = m_lua_modules.find(m);
        if (it != m_lua_modules.end()) {
            return it->second;
        } else {
            if (m.is_root()) {
                auto mod = std::make_shared<lua_module>(*this, m);
                return m_lua_modules[m] = mod;
            } else {
                lua_module_ref r;
                return r;
            }
        }
    }

} // namespace mge::lua