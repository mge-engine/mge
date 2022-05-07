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
            : exec(false)
            , libraries(false)
            , interactive(false)
            , show_version(false)
            , ignore_env(false)
            , warnings(false)
            , exec_stdin(false)
            , script_pos(-1)
        {}

        bool                     exec;
        bool                     libraries;
        bool                     interactive;
        bool                     show_version;
        bool                     ignore_env;
        bool                     warnings;
        bool                     exec_stdin;
        int                      script_pos;
        std::string              script;
        std::vector<std::string> script_args;

        bool nothing_to_do() const
        {
            return interactive == false && script.empty() &&
                   exec_stdin == false;
        }
    };

    static void print_usage()
    {
        std::cout
            << "Usage: mgelua [options] [script [args]]\n"
               "Available options are:\n"
               "  -e stat   execute string 'stat'\n"
               "  -i        enter interactive mode after executing 'script'\n"
               "  -l mod    require library 'mod' into global 'mod'\n"
               "  -l g=mod  require library 'mod' into global 'g'\n"
               "  -v        show version information\n"
               "  -E        ignore environment variables\n"
               "  -W        turn warnings on\n"
               "  --        stop handling options\n"
               "  -         stop handling options and execute stdin\n";
    }

    static void print_version() { std::cout << LUA_COPYRIGHT << std::endl; }

    static void
    parse_options(int argc, const char** argv, interpreter_options& options)
    {
        int i = 1;
        while (i < argc) {
            options.script_pos = i;
            if (argv[i][0] != '-') {
                options.script = argv[i];
                ++i;
                for (int j = i; j < argc; ++j) {
                    options.script_args.emplace_back(argv[j]);
                }
                return;
            } else {
                switch (argv[i][1]) {
                case '-': // --
                    if (argv[i][2]) {
                        MGE_THROW(mge::exception)
                            << "Invalid option: " << argv[i];
                    }
                    ++i;
                    options.script_pos = i;
                    if (i < argc) {
                        options.script = argv[i];
                        ++i;
                        for (int j = i; j < argc; ++j) {
                            options.script_args.emplace_back(argv[j]);
                        }
                    }
                    return;
                case '\0': // -
                    options.exec_stdin = true;
                    options.script = "-";
                    ++i;
                    if (i < argc) {
                        for (int j = i; j < argc; ++j) {
                            options.script_args.emplace_back(argv[j]);
                        }
                    }
                    return;
                case 'E':
                    if (argv[i][2]) {
                        MGE_THROW(mge::exception)
                            << "Invalid option: " << argv[i];
                    }
                    options.ignore_env = true;
                    break;
                case 'W':
                    if (argv[i][2]) {
                        MGE_THROW(mge::exception)
                            << "Invalid option: " << argv[i];
                    }
                    options.warnings = true;
                    break;
                case 'i':
                    if (argv[i][2]) {
                        MGE_THROW(mge::exception)
                            << "Invalid option: " << argv[i];
                    }
                    options.interactive = true;
                    options.show_version = true;
                    break;
                case 'v':
                    if (argv[i][2]) {
                        MGE_THROW(mge::exception)
                            << "Invalid option: " << argv[i];
                    }
                    options.show_version = true;
                    break;
                case 'e':
                    if (argv[i][2]) {
                        options.exec = true;
                    } else {
                        ++i;
                        if (i < argc) {
                            if (argv[i][0] == '-') {
                                MGE_THROW(mge::exception)
                                    << "Option '-e' requires an argument";
                            }
                            options.exec = true;
                        } else {
                            MGE_THROW(mge::exception)
                                << "Option '-e' requires an argument";
                        }
                    }
                    break;
                case 'l':
                    if (argv[i][2]) {
                        options.libraries = true;
                    } else {
                        ++i;
                        if (i < argc) {
                            if (argv[i][0] == '-') {
                                MGE_THROW(mge::exception)
                                    << "Option '-l' requires an argument";
                            }
                            options.libraries = true;
                        } else {
                            MGE_THROW(mge::exception)
                                << "Option '-l' requires an argument";
                        }
                    }
                    break;
                default: {
                    MGE_THROW(mge::exception) << "Invalid option: " << argv[i];
                }
                }
            }
            ++i;
        }
    }

    static void createargtable(lua_State*                 L,
                               int                        argc,
                               const char**               argv,
                               const interpreter_options& options)
    {
        int script = options.script_pos;
        if (script == argc || script == -1) {
            script = 0;
        }
        int narg = argc - (script + 1); /* number of positive indices */
        lua_createtable(L, narg, script + 1);
        for (int i = 0; i < argc; i++) {
            lua_pushstring(L, argv[i]);
            lua_rawseti(L, -2, i - script);
        }
        lua_setglobal(L, "arg");
    }

    static int handle_message(lua_State* L)
    {
        const char* msg = lua_tostring(L, 1);
        if (msg == nullptr) { /* is error object not a string? */
            if (luaL_callmeta(L,
                              1,
                              "__tostring") &&  /* does it have a metamethod */
                lua_type(L, -1) == LUA_TSTRING) /* that produces a string? */
                return 1;                       /* that is the message */
            else
                msg = lua_pushfstring(L,
                                      "(error object is a %s value)",
                                      luaL_typename(L, 1));
        }
        luaL_traceback(L, L, msg, 1); /* append a standard traceback */
        return 1;                     /* return the traceback */
    }

    static int call(lua_State* L, int nargs, int nresults)
    {
        int base = lua_gettop(L) - nargs;
        lua_pushcfunction(L, handle_message);
        lua_insert(L, base);
        int status = lua_pcall(L, nargs, nresults, base);
        lua_remove(L, base);
        return status;
    }

    static int report(lua_State* L, int status)
    {
        try {
            CHECK_STATUS(status, L);
        } catch (const mge::exception& ex) {
            std::cerr << ex << std::endl;
        }
        lua_pop(L, 1); /* remove message */
        return status;
    }

    static int eval_chunk(lua_State* L, int status)
    {
        if (status == LUA_OK) {
            status = call(L, 0, 0);
        }
        return report(L, status);
    }

    static int eval_file(lua_State* L, const char* name)
    {
        return eval_chunk(L, luaL_loadfile(L, name));
    }

    static int eval_string(lua_State* L, const char* code, const char* name)
    {
        return eval_chunk(L, luaL_loadbuffer(L, code, strlen(code), name));
    }

    static int init(lua_State* L)
    {
#define LUA_INIT_VAR "LUA_INIT"
#define LUA_INITVARVERSION LUA_INIT_VAR LUA_VERSUFFIX
        const char* name = "=" LUA_INITVARVERSION;
        const char* init = getenv(name + 1);
        if (init == nullptr) {
            name = "=" LUA_INIT_VAR;
            init = getenv(name + 1); /* try alternative name */
        }
        if (init == nullptr) {
            return LUA_OK;
        }
        if (init[0] == '@') {
            return eval_file(L, init + 1);
        } else {
            return eval_string(L, init, name + 1);
        }

#undef LUA_INITVARVERSION
#undef LUA_INIT_VAR
    }

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
        }

        if (options.nothing_to_do()) {
            return 0;
        }

        if (options.ignore_env) {
            lua_pushboolean(m_lua_state,
                            1); /* signal for libraries to ignore env. vars. */
            lua_setfield(m_lua_state, LUA_REGISTRYINDEX, "LUA_NOENV");
        }

        createargtable(m_lua_state, argc, argv, options);
        lua_gc(m_lua_state, LUA_GCGEN, 0, 0); /* GC in generational mode */

        if (!options.ignore_env) {
            if (init(m_lua_state) != LUA_OK) {
                return 1;
            }
        }
#if 0
  if (!runargs(L, argv, script))  /* execute arguments -e and -l */
    return 0;  /* something failed */
  if (script < argc &&  /* execute main script (if there is one) */
      handle_script(L, argv + script) != LUA_OK)
    return 0;
  if (args & has_i)  /* -i option? */
    doREPL(L);  /* do read-eval-print loop */
  else if (script == argc && !(args & (has_e | has_v))) {  /* no arguments? */
    if (lua_stdin_is_tty()) {  /* running in interactive mode? */
      print_version();
      doREPL(L);  /* do read-eval-print loop */
    }
    else dofile(L, NULL);  /* executes stdin as a file */
  }
  lua_pushboolean(L, 1);  /* signal no errors */
  return 1;
#endif
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