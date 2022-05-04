// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.

#include "lua_context.hpp"
#include "lua_binder.hpp"
#include "lua_error.hpp"
#include "lua_module.hpp"

#include "mge/core/line_editor.hpp"

#include <string>
#include <string_view>
#include <vector>

#ifdef MGE_OS_WINDOWS
#    include <io.h>
#    include <windows.h>
#    define lua_stdin_is_tty() _isatty(_fileno(stdin))
#else
#    error missing port
#endif

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

    /* bits of various argument indicators in 'args' */
    static constexpr int has_error = 1; /* bad option */
    static constexpr int has_i = 2;     /* -i */
    static constexpr int has_v = 4;     /* -v */
    static constexpr int has_e = 8;     /* -e */
    static constexpr int has_E = 16;    /* -E */

    static lua_State* globalL = nullptr;

    static void l_message(const char* pname, const char* msg)
    {
        if (pname)
            lua_writestringerror("%s: ", pname);
        lua_writestringerror("%s\n", msg);
    }

    static int report(lua_State* L, int status)
    {
        const char* progname = "mgelua";
        if (status != LUA_OK) {
            const char* msg = lua_tostring(L, -1);
            l_message(progname, msg);
            lua_pop(L, 1); /* remove message */
        }
        return status;
    }

    static int collectargs(char** argv, int* first)
    {
        int args = 0;
        int i;
        for (i = 1; argv[i] != NULL; i++) {
            *first = i;
            if (argv[i][0] != '-')      /* not an option? */
                return args;            /* stop handling options */
            switch (argv[i][1]) {       /* else check option */
            case '-':                   /* '--' */
                if (argv[i][2] != '\0') /* extra characters after '--'? */
                    return has_error;   /* invalid option */
                *first = i + 1;
                return args;
            case '\0':       /* '-' */
                return args; /* script "name" is '-' */
            case 'E':
                if (argv[i][2] != '\0') /* extra characters? */
                    return has_error;   /* invalid option */
                args |= has_E;
                break;
            case 'W':
                if (argv[i][2] != '\0') /* extra characters? */
                    return has_error;   /* invalid option */
                break;
            case 'i':
                args |= has_i; /* (-i implies -v) */ /* FALLTHROUGH */
            case 'v':
                if (argv[i][2] != '\0') /* extra characters? */
                    return has_error;   /* invalid option */
                args |= has_v;
                break;
            case 'e':
                args |= has_e;            /* FALLTHROUGH */
            case 'l':                     /* both options need an argument */
                if (argv[i][2] == '\0') { /* no concatenated argument? */
                    i++;                  /* try next 'argv' */
                    if (argv[i] == NULL || argv[i][0] == '-')
                        return has_error; /* no next argument or it is another
                                             option */
                }
                break;
            default: /* invalid option */
                return has_error;
            }
        }
        *first = i; /* no script name */
        return args;
    }

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

    static void createargtable(lua_State* L, char** argv, int argc, int script)
    {
        int i, narg;
        if (script == argc)
            script = 0;             /* no script name? */
        narg = argc - (script + 1); /* number of positive indices */
        lua_createtable(L, narg, script + 1);
        for (i = 0; i < argc; i++) {
            lua_pushstring(L, argv[i]);
            lua_rawseti(L, -2, i - script);
        }
        lua_setglobal(L, "arg");
    }

#if !defined(LUA_INIT_VAR)
#    define LUA_INIT_VAR "LUA_INIT"
#endif

#define LUA_INITVARVERSION LUA_INIT_VAR LUA_VERSUFFIX

    static int msghandler(lua_State* L)
    {
        const char* msg = lua_tostring(L, 1);
        if (msg == NULL) { /* is error object not a string? */
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

    static int docall(lua_State* L, int narg, int nres)
    {
        int status;
        int base = lua_gettop(L) - narg;  /* function index */
        lua_pushcfunction(L, msghandler); /* push message handler */
        lua_insert(L, base);              /* put it under function and args */
        globalL = L;                      /* to be available to 'laction' */
        // setsignal(SIGINT, laction);       /* set C-signal handler */
        status = lua_pcall(L, narg, nres, base);
        // setsignal(SIGINT, SIG_DFL); /* reset C-signal handler */
        lua_remove(L, base); /* remove message handler from the stack */
        return status;
    }

    static int dochunk(lua_State* L, int status)
    {
        if (status == LUA_OK)
            status = docall(L, 0, 0);
        return report(L, status);
    }

    static int dofile(lua_State* L, const char* name)
    {
        return dochunk(L, luaL_loadfile(L, name));
    }

    static int dostring(lua_State* L, const char* s, const char* name)
    {
        return dochunk(L, luaL_loadbuffer(L, s, strlen(s), name));
    }

    static int handle_luainit(lua_State* L)
    {
        const char* name = "=" LUA_INITVARVERSION;
        const char* init = getenv(name + 1);
        if (init == NULL) {
            name = "=" LUA_INIT_VAR;
            init = getenv(name + 1); /* try alternative name */
        }
        if (init == NULL)
            return LUA_OK;
        else if (init[0] == '@')
            return dofile(L, init + 1);
        else
            return dostring(L, init, name);
    }

    static void lstop(lua_State* L, lua_Debug* ar)
    {
        (void)ar;                   /* unused arg. */
        lua_sethook(L, NULL, 0, 0); /* reset hook */
        luaL_error(L, "interrupted!");
    }

    void laction(int i)
    {
        int flag = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE | LUA_MASKCOUNT;
#if 0
        setsignal(i,
                  SIG_DFL); /* if another SIGINT happens, terminate process */
#endif
        lua_sethook(globalL, lstop, flag, 1);
    }

    static int dolibrary(lua_State* L, char* globname)
    {
        int   status;
        char* modname = strchr(globname, '=');
        if (modname == NULL)    /* no explicit name? */
            modname = globname; /* module name is equal to global name */
        else {
            *modname = '\0'; /* global name ends here */
            modname++;       /* module name starts after the '=' */
        }
        lua_getglobal(L, "require");
        lua_pushstring(L, modname);
        status = docall(L, 1, 1); /* call 'require(modname)' */
        if (status == LUA_OK)
            lua_setglobal(L, globname); /* globname = require(modname) */
        return report(L, status);
    }

    /*
     ** Processes options 'e' and 'l', which involve running Lua code, and
     ** 'W', which also affects the state.
     ** Returns 0 if some code raises an error.
     */
    static int runargs(lua_State* L, char** argv, int n)
    {
        int i;
        for (i = 1; i < n; i++) {
            int option = argv[i][1];
            lua_assert(argv[i][0] == '-'); /* already checked */
            switch (option) {
            case 'e':
            case 'l': {
                int   status;
                char* extra = argv[i] + 2; /* both options need an argument */
                if (*extra == '\0')
                    extra = argv[++i];
                lua_assert(extra != NULL);
                status = (option == 'e') ? dostring(L, extra, "=(command line)")
                                         : dolibrary(L, extra);
                if (status != LUA_OK)
                    return 0;
                break;
            }
            case 'W':
                lua_warning(L, "@on", 0); /* warnings on */
                break;
            }
        }
        return 1;
    }

    /*
    ** Push on the stack the contents of table 'arg' from 1 to #arg
    */
    static int pushargs(lua_State* L)
    {
        int i, n;
        if (lua_getglobal(L, "arg") != LUA_TTABLE)
            luaL_error(L, "'arg' is not a table");
        n = (int)luaL_len(L, -1);
        luaL_checkstack(L, n + 3, "too many arguments to script");
        for (i = 1; i <= n; i++)
            lua_rawgeti(L, -i, i);
        lua_remove(L, -i); /* remove table from the stack */
        return n;
    }

    static int handle_script(lua_State* L, char** argv)
    {
        int         status;
        const char* fname = argv[0];
        if (strcmp(fname, "-") == 0 && strcmp(argv[-1], "--") != 0)
            fname = NULL; /* stdin */
        status = luaL_loadfile(L, fname);
        if (status == LUA_OK) {
            int n = pushargs(L); /* push arguments to script */
            status = docall(L, n, LUA_MULTRET);
        }
        return report(L, status);
    }

    /*
     ** Do the REPL: repeatedly read (load) a line, evaluate (call) it, and
     ** print any results.
     */
    static void doREPL(lua_State* L)
    {
#if 0
        int         status;
        const char* oldprogname = progname;
        progname = NULL; /* no 'progname' on errors in interactive mode */
        lua_initreadline(L);
        while ((status = loadline(L)) != -1) {
            if (status == LUA_OK)
                status = docall(L, 0, LUA_MULTRET);
            if (status == LUA_OK)
                l_print(L);
            else
                report(L, status);
        }
        lua_settop(L, 0); /* clear stack */
        lua_writeline();
        progname = oldprogname;
#endif
    }

    static int pmain(lua_State* L)
    {
        int         argc = (int)lua_tointeger(L, 1);
        char**      argv = (char**)lua_touserdata(L, 2);
        int         script;
        int         args = collectargs(argv, &script);
        const char* progname = "mgelua";

        luaL_checkversion(L); /* check that interpreter has correct version */

        if (argv[0] && argv[0][0])
            progname = argv[0];
        if (args == has_error) { /* bad arg? */
            print_usage(progname,
                        argv[script]); /* 'script' has index of bad arg. */
            return 0;
        }
        if (args & has_v) /* option '-v'? */
            print_version();
        if (args & has_E) { /* option '-E'? */
            lua_pushboolean(L,
                            1); /* signal for libraries to ignore env. vars. */
            lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
        }
        createargtable(L, argv, argc, script); /* create table 'arg' */
        lua_gc(L, LUA_GCGEN, 0, 0);            /* GC in generational mode */
        if (!(args & has_E)) {                 /* no option '-E'? */
            if (handle_luainit(L) != LUA_OK)   /* run LUA_INIT */
                return 0;                      /* error running LUA_INIT */
        }
        if (!runargs(L, argv, script)) /* execute arguments -e and -l */
            return 0;                  /* something failed */
        if (script < argc && /* execute main script (if there is one) */
            handle_script(L, argv + script) != LUA_OK)
            return 0;
        if (args & has_i) /* -i option? */
            doREPL(L);    /* do read-eval-print loop */
        else if (script == argc &&
                 !(args & (has_e | has_v))) { /* no arguments? */
            if (lua_stdin_is_tty()) {         /* running in interactive mode? */
                print_version();
                doREPL(L); /* do read-eval-print loop */
            } else
                dofile(L, NULL); /* executes stdin as a file */
        }
        lua_pushboolean(L, 1); /* signal no errors */
        return 1;
    }

    int lua_context::main(int argc, const char** argv)
    {
        lua_pushcfunction(m_lua_state, &pmain);
        lua_pushinteger(m_lua_state, argc);
        lua_pushlightuserdata(m_lua_state, argv);
        int status = lua_pcall(m_lua_state, 2, 1, 0);
        int result = lua_toboolean(m_lua_state, -1);
        CHECK_STATUS(status, m_lua_state);
        return result ? 0 : 1;
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