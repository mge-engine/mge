// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "lua_context.hpp"
#include "lua_binder.hpp"
#include "lua_error.hpp"
#include "mge/core/line_editor.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/module_details.hpp"
#include "mge/reflection/type_details.hpp"

#include "mge/core/crash.hpp"

#if !defined(lua_writestringerror)
#    define lua_writestringerror(s, p)                                         \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif

#if !defined(lua_writestring)
#    define lua_writestring(s, l) fwrite((s), sizeof(char), (l), stdout)
#endif

#if !defined(lua_writeline)
#    define lua_writeline() (lua_writestring("\n", 1), fflush(stdout))
#endif

namespace mge {
    MGE_USE_TRACE(LUA);
}

#ifdef MGE_OS_WINDOWS
#    include <io.h>
#    include <stdio.h>
static inline bool lua_stdin_is_tty()
{
    return _isatty(_fileno(stdin)) != 0;
}
#elif defined(MGE_OS_LINUX) || defined(MGE_OS_MACOSX)
#    include <unistd.h>
static inline bool lua_stdin_is_tty()
{
    return isatty(fileno(stdin)) != 0;
}
#else
#    error missing port
#endif

namespace mge::lua {

    namespace {
        // Minimal call_context for invoking a default constructor
        // via the reflection invoke_function. Only this_ptr() is
        // meaningful; all parameter/result methods abort.
        class default_ctor_context : public mge::reflection::call_context
        {
        public:
            explicit default_ctor_context(void* ptr)
                : m_ptr(ptr)
            {}

            void* this_ptr() override
            {
                return m_ptr;
            }

            // None of the parameter/result methods should be called
            // for a default constructor.
            // clang-format off
            bool             bool_parameter(size_t) override { mge::crash(); }
            int8_t           int8_t_parameter(size_t) override { mge::crash(); }
            uint8_t          uint8_t_parameter(size_t) override { mge::crash(); }
            int16_t          int16_t_parameter(size_t) override { mge::crash(); }
            uint16_t         uint16_t_parameter(size_t) override { mge::crash(); }
            int32_t          int32_t_parameter(size_t) override { mge::crash(); }
            uint32_t         uint32_t_parameter(size_t) override { mge::crash(); }
            int64_t          int64_t_parameter(size_t) override { mge::crash(); }
            uint64_t         uint64_t_parameter(size_t) override { mge::crash(); }
            float            float_parameter(size_t) override { mge::crash(); }
            double           double_parameter(size_t) override { mge::crash(); }
            long double      long_double_parameter(size_t) override { mge::crash(); }
            std::string_view string_view_parameter(size_t) override { mge::crash(); }
            void*            pointer_parameter(size_t, const mge::reflection::type_details&) override { mge::crash(); }
            void bool_result(bool) override { mge::crash(); }
            void int8_t_result(int8_t) override { mge::crash(); }
            void uint8_t_result(uint8_t) override { mge::crash(); }
            void int16_t_result(int16_t) override { mge::crash(); }
            void uint16_t_result(uint16_t) override { mge::crash(); }
            void int32_t_result(int32_t) override { mge::crash(); }
            void uint32_t_result(uint32_t) override { mge::crash(); }
            void int64_t_result(int64_t) override { mge::crash(); }
            void uint64_t_result(uint64_t) override { mge::crash(); }
            void float_result(float) override { mge::crash(); }
            void double_result(double) override { mge::crash(); }
            void long_double_result(long double) override { mge::crash(); }
            void string_view_result(std::string_view) override { mge::crash(); }
            void pointer_result(void*) override { mge::crash(); }
            void shared_ptr_result(std::shared_ptr<void>) override { mge::crash(); }
            void primitive_vector_result(const void*, size_t, const std::type_index&) override { mge::crash(); }
            void primitive_vector_parameter(size_t, void*, const std::type_index&) override { mge::crash(); }
            void exception_thrown(const mge::exception&) override { mge::crash(); }
            void exception_thrown(const std::exception&) override { mge::crash(); }
            void exception_thrown() override { mge::crash(); }
            // clang-format on
        private:
            void* m_ptr;
        };
    } // namespace

    lua_context::lua_context(lua_engine* engine)
        : m_engine(engine)
        , m_lua_state(nullptr)
    {
        MGE_DEBUG_TRACE(LUA, "Creating new Lua state");
        m_lua_state = luaL_newstate();
        CHECK_STATUS(m_lua_state ? LUA_OK : LUA_ERRMEM, m_lua_state);
        MGE_DEBUG_TRACE(LUA, "Lua state created successfully");
        luaL_openlibs(m_lua_state);
        MGE_DEBUG_TRACE(LUA, "Lua libs opened successfully");
    }

    lua_context::~lua_context()
    {
        for (auto& entry : m_component_entries) {
            entry->unregister();
        }
        m_component_entries.clear();
        if (m_lua_state) {
            lua_close(m_lua_state);
            m_lua_state = nullptr;
        }
    }

    void lua_context::eval(const std::string& code)
    {
        int rc = luaL_loadbuffer(m_lua_state, code.data(), code.size(), "");
        CHECK_STATUS(rc, m_lua_state);
        rc = lua_pcall(m_lua_state, 0, 0, 0);
        CHECK_STATUS(rc, m_lua_state);
    }

    void lua_context::bind()
    {
        reflection::module root_module = reflection::module::root();
        MGE_DEBUG_TRACE(LUA, "Compute binding information");
        create_helper_module();

        lua_binder b(this);
        root_module.details()->apply(b);

        register_class_function();
        register_component_function();
    }

    void lua_context::register_class_function()
    {
        auto L = lua_state();
        // clang-format off
        const char* code =
            "function class(base)\n"
            "    local c = {}\n"
            "    if base then\n"
            "        setmetatable(c, { __index = base })\n"
            "        c.super = base\n"
            "    end\n"
            "    c.__index = function(t, k)\n"
            "        local v = c[k]\n"
            "        if v ~= nil then return v end\n"
            "        local native = rawget(t, '_native')\n"
            "        if native then return native[k] end\n"
            "    end\n"
            "    function c.new(...)\n"
            "        local self = setmetatable({}, c)\n"
            "        if self.init then\n"
            "            self:init(...)\n"
            "        end\n"
            "        return self\n"
            "    end\n"
            "    return c\n"
            "end\n";
        // clang-format on
        int rc = luaL_dostring(L, code);
        CHECK_STATUS(rc, L);
        MGE_DEBUG_TRACE(LUA, "Registered class function");
    }

    void lua_context::register_component_function()
    {
        auto L = lua_state();
        // set mge.register_component = closure(component_call, upvalue=this)
        lua_getglobal(L, "mge");
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &lua_context::component_call, 1);
        lua_setfield(L, -2, "register_component");
        lua_pop(L, 1); // pop mge table
        MGE_DEBUG_TRACE(LUA, "Registered component function");
    }

    // Separated from component_call so that C++ objects with
    // destructors (std::string, std::function, unique_ptr) never
    // share a stack frame with luaL_error. LuaJIT uses SEH-based
    // unwinding on Windows, which invokes destructors for all
    // locals in a function frame — including uninitialized ones
    // declared after the error point.
    static void do_register_component(
        std::vector<std::unique_ptr<
            mge::dynamic_implementation_registry_entry>>& entries,
        const mge::reflection::type_details*              details,
        const mge::reflection::type_details*              proxy_type,
        const char*                                       impl_name_cstr)
    {
        const auto& proxy_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                proxy_type->specific_details);

        // find default constructor (0 args) on proxy type
        const mge::reflection::invoke_function* default_ctor = nullptr;
        for (const auto& [sig, invoke_fn] : proxy_details.constructors) {
            if (sig.parameter_types().empty()) {
                default_ctor = &invoke_fn;
                break;
            }
        }
        if (!default_ctor) {
            MGE_THROW(mge::illegal_state) << "proxy type '" << proxy_type->name
                                          << "' has no default constructor";
        }

        std::string impl_name(impl_name_cstr);
        std::string component_name(details->name);

        auto ctor_fn = *default_ctor;
        auto raw_dtor = proxy_details.raw_destructor;
        auto proxy_size = proxy_type->size;

        auto factory = [ctor_fn,
                        raw_dtor,
                        proxy_size]() -> std::shared_ptr<mge::component_base> {
            void*                raw = ::operator new(proxy_size);
            default_ctor_context ctx(raw);
            ctor_fn(ctx);
            auto shared = std::shared_ptr<mge::component_base>(
                static_cast<mge::component_base*>(raw),
                [raw_dtor](mge::component_base* p) {
                    if (raw_dtor) {
                        raw_dtor(p);
                    }
                    ::operator delete(p);
                });
            return shared;
        };

        auto entry =
            std::make_unique<mge::dynamic_implementation_registry_entry>(
                std::move(factory),
                component_name,
                impl_name);
        entries.push_back(std::move(entry));

        MGE_DEBUG_TRACE(LUA,
                        "Registered component '{}' implementation '{}'",
                        component_name,
                        impl_name);
    }

    int lua_context::component_call(lua_State* L)
    {
        auto* self =
            static_cast<lua_context*>(lua_touserdata(L, lua_upvalueindex(1)));

        // arg1: base type table (C++ class with proxy)
        if (!lua_istable(L, 1)) {
            return luaL_error(L, "mge.component arg1: expected a type table");
        }

        lua_getfield(L, 1, "__details__");
        auto* details = static_cast<const mge::reflection::type_details*>(
            lua_touserdata(L, -1));
        lua_pop(L, 1);

        if (!details || !details->is_class) {
            return luaL_error(
                L,
                "mge.component arg1: expected a class type table");
        }

        const auto& class_details =
            std::get<mge::reflection::type_details::class_specific_details>(
                details->specific_details);

        const mge::reflection::type_details* proxy_type =
            class_details.proxy_type.get();
        if (!proxy_type) {
            return luaL_error(L, "mge.component arg1: type has no proxy");
        }

        // arg2: derived Lua class table (created by class(base))
        if (!lua_istable(L, 2)) {
            return luaL_error(L, "mge.component arg2: expected a class table");
        }

        // verify arg2 is derived from arg1 via class():
        //   class(base) sets setmetatable(c, { __index = base })
        //   so getmetatable(arg2).__index should be arg1
        if (!lua_getmetatable(L, 2)) {
            return luaL_error(L,
                              "mge.component arg2: table has no metatable"
                              " (not created by class())");
        }
        lua_getfield(L, -1, "__index");
        bool is_derived = lua_rawequal(L, -1, 1) != 0;
        lua_pop(L, 2); // pop __index and metatable

        if (!is_derived) {
            return luaL_error(L,
                              "mge.component arg2: not derived from arg1"
                              " via class()");
        }

        // get implementation name from arg2.__name__
        lua_getfield(L, 2, "__name__");
        if (!lua_isstring(L, -1)) {
            return luaL_error(L,
                              "mge.component arg2: missing '__name__' string"
                              " field");
        }
        const char* impl_name_cstr = lua_tostring(L, -1);
        lua_pop(L, 1);

        do_register_component(self->m_component_entries,
                              details,
                              proxy_type,
                              impl_name_cstr);
        return 0;
    }

    void lua_context::create_helper_module()
    {
        auto L = lua_state();
        // create global table named '__mge__'
        lua_newtable(L);
        lua_pushvalue(L, -1);        // duplicate the table
        lua_setglobal(L, "__mge__"); // set as global variable
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");
        lua_pushvalue(L, -3);           // push the table again
        lua_setfield(L, -2, "__mge__"); // package.loaded["__mge__"] = table
        lua_pop(L, 3); // pop package.loaded, package, and table
    }

/* bits of various argument indicators in 'args' */
#define has_error 1 /* bad option */
#define has_i 2     /* -i */
#define has_v 4     /* -v */
#define has_e 8     /* -e */
#define has_E 16    /* -E */

    /*
     ** Prints an error message, adding the program name in front of it
     ** (if present)
     */
    static void l_message(const char* pname, const char* msg)
    {
        if (pname)
            lua_writestringerror("%s: ", pname);
        lua_writestringerror("%s\n", msg);
    }

    /*
    ** Check whether 'status' is not OK and, if so, prints the error
    ** message on the top of the stack. It assumes that the error object
    ** is a string, as it was either generated by Lua or by 'msghandler'.
    */
    static int report(lua_State* L, int status, const char* progname = nullptr)
    {
        if (status != LUA_OK) {
            const char* msg = lua_tostring(L, -1);
            l_message(progname, msg);
            lua_pop(L, 1); /* remove message */
        }
        return status;
    }

    /*
    ** Message handler used to run all chunks
    */
    static int msghandler(lua_State* L)
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

    /*
    ** Interface to 'lua_pcall', which sets appropriate message function
    ** and C-signal handler. Used to run all chunks.
    */
    static int docall(lua_State* L, int narg, int nres)
    {
        int status;
        int base = lua_gettop(L) - narg;  /* function index */
        lua_pushcfunction(L, msghandler); /* push message handler */
        lua_insert(L, base);              /* put it under function and args */
        status = lua_pcall(L, narg, nres, base);
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

    /*
     ** Push on the stack the contents of table 'arg' from 1 to #arg
     */
    static int pushargs(lua_State* L)
    {
        int i, n;
        lua_getglobal(L, "arg");
        if (lua_type(L, -1) != LUA_TTABLE)
            luaL_error(L, "'arg' is not a table");
        n = (int)lua_objlen(L, -1);
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
            fname = nullptr; /* stdin */
        status = luaL_loadfile(L, fname);
        if (status == LUA_OK) {
            int n = pushargs(L); /* push arguments to script */
            status = docall(L, n, LUA_MULTRET);
        }
        return report(L, status);
    }

    /*
     ** Receives 'globname[=modname]' and runs 'globname = require(modname)'.
     */
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
                /* warnings not supported in Lua 5.1/LuaJIT */
                break;
            }
        }
        return 1;
    }

    static int handle_luainit(lua_State* L)
    {
        std::string name("LUA_INIT_5_1");
        const char* init = getenv(name.c_str());
        if (init == nullptr) {
            name = "LUA_INIT";
            init = getenv(name.c_str()); /* try alternative name */
        }
        if (init == nullptr)
            return LUA_OK;
        else if (init[0] == '@')
            return dofile(L, init + 1);
        else {
            name = std::string("=") + name;
            return dostring(L, init, name.c_str());
        }
    }

    static void print_version(void)
    {
        lua_writestring(LUA_COPYRIGHT, strlen(LUA_COPYRIGHT));
        lua_writeline();
    }

    /*
    ** Create the 'arg' table, which stores all arguments from the
    ** command line ('argv'). It should be aligned so that, at index 0,
    ** it has 'argv[script]', which is the script name. The arguments
    ** to the script (everything after 'script') go to positive indices;
    ** other arguments (before the script name) go to negative indices.
    ** If there is no script name, assume interpreter's name as base.
    */
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
            //            "  -W        turn warnings on\n"
            "  --        stop handling options\n"
            "  -         stop handling options and execute stdin\n",
            progname);
    }

    /*
    ** Traverses all arguments from 'argv', returning a mask with those
    ** needed before running any Lua code (or an error code if it finds
    ** any invalid argument). 'first' returns the first not-handled argument
    ** (either the script name or a bad argument in case of error).
    */
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

/* mark in error messages for incomplete statements */
#define EOFMARK "<eof>"
#define marklen (sizeof(EOFMARK) / sizeof(char) - 1)

    /*
     ** Check whether 'status' signals a syntax error and the error
     ** message at the top of the stack ends with the above mark for
     ** incomplete statements.
     */
    static int incomplete(lua_State* L, int status)
    {
        if (status == LUA_ERRSYNTAX) {
            size_t      lmsg;
            const char* msg = lua_tolstring(L, -1, &lmsg);
            if (lmsg >= marklen && strcmp(msg + lmsg - marklen, EOFMARK) == 0) {
                lua_pop(L, 1);
                return 1;
            }
        }
        return 0; /* else... */
    }

    /*
     ** Try to compile line on the stack as 'return <line>;'; on return, stack
     ** has either compiled chunk or original line (if compilation failed).
     */
    static int addreturn(lua_State* L)
    {
        const char* line = lua_tostring(L, -1); /* original line */
        const char* retline = lua_pushfstring(L, "return %s;", line);
        int status = luaL_loadbuffer(L, retline, strlen(retline), "=stdin");
        if (status == LUA_OK) {
            lua_remove(L, -2); /* remove modified line */
        } else
            lua_pop(
                L,
                2); /* pop result from 'luaL_loadbuffer' and modified line */
        return status;
    }

#define LUA_PROMPT "> "
#define LUA_PROMPT2 ">> "

    /*
    ** Return the string to be used as a prompt by the interpreter. Leave
    ** the string (or nil, if using the default value) on the stack, to keep
    ** it anchored.
    */
    static const char* get_prompt(lua_State* L, int firstline)
    {
        lua_getglobal(L, firstline ? "_PROMPT" : "_PROMPT2");
        if (lua_type(L, -1) == LUA_TNIL)
            return (firstline ? LUA_PROMPT : LUA_PROMPT2); /* use the default */
        else { /* apply 'tostring' over the value */
            const char* p = lua_tostring(L, -1);
            if (p == NULL)
                p = (firstline ? LUA_PROMPT : LUA_PROMPT2);
            return p;
        }
    }

    /*
     ** Prompt the user, read a line, and push it into the Lua stack.
     */
    static int pushline(lua_State* L, int firstline, line_editor& edit)
    {
        const char* prmt = get_prompt(L, firstline);
        edit.set_prompt(prmt);

        auto line = edit.line();
        if (!line) {
            return 0; /* no input (prompt will be popped by caller) */
        }
        lua_pop(L, 1); /* remove prompt */
        const char* b = line.value().c_str();
        size_t      l = line.value().size();
        if (l > 0 && b[l - 1] == '\n') /* line ends with newline? */
            line.value().resize(--l);  /* remove it */

        if (firstline && b[0] == '=') /* for compatibility with 5.2, ... */
            lua_pushfstring(L, "return %s", b + 1); /* change '=' to 'return' */
        else
            lua_pushlstring(L, b, l);
        return 1;
    }

    /*
     ** Read multiple lines until a complete Lua statement
     */
    static int multiline(lua_State* L, line_editor& editor)
    {
        for (;;) { /* repeat until gets a complete statement */
            size_t      len;
            const char* line = lua_tolstring(L, 1, &len); /* get what it has */
            int status = luaL_loadbuffer(L, line, len, "=stdin"); /* try it */
            if (!incomplete(L, status) || !pushline(L, 0, editor)) {
                return status; /* cannot or should not try to add continuation
                                  line */
            }
            lua_pushliteral(L, "\n"); /* add newline... */
            lua_insert(L, -2);        /* ...between the two lines */
            lua_concat(L, 3);         /* join them */
        }
    }

    /*
    ** Read a line and try to load (compile) it first as an expression (by
    ** adding "return " in front of it) and second as a statement. Return
    ** the final status of load/call with the resulting function (if any)
    ** in the top of the stack.
    */
    static int loadline(lua_State* L, line_editor& editor)
    {
        int status;
        lua_settop(L, 0);
        if (!pushline(L, 1, editor))
            return -1;                         /* no input */
        if ((status = addreturn(L)) != LUA_OK) /* 'return ...' did not work? */
            status = multiline(L, editor);
        /* try as command, maybe with continuation lines */
        lua_remove(L, 1); /* remove line from the stack */
        lua_assert(lua_gettop(L) == 1);
        return status;
    }

    /*
    ** Prints (calling the Lua 'print' function) any values on the stack
    */
    static void l_print(lua_State* L, const char* progname)
    {
        int n = lua_gettop(L);
        if (n > 0) { /* any result to be printed? */
            luaL_checkstack(L, LUA_MINSTACK, "too many results to print");
            lua_getglobal(L, "print");
            lua_insert(L, 1);
            if (lua_pcall(L, n, 0, 0) != LUA_OK)
                l_message(progname,
                          lua_pushfstring(L,
                                          "error calling 'print' (%s)",
                                          lua_tostring(L, -1)));
        }
    }

    /*
    ** Do the REPL: repeatedly read (load) a line, evaluate (call) it, and
    ** print any results.
    */
    static void doREPL(lua_State* L, const char* progname)
    {
        int         status;
        line_editor editor;

        while ((status = loadline(L, editor)) != -1) {
            if (status == LUA_OK)
                status = docall(L, 0, LUA_MULTRET);
            if (status == LUA_OK)
                l_print(L, progname);
            else
                report(L, status);
        }
        lua_settop(L, 0); /* clear stack */
        lua_writeline();
    }

    /*
    ** Main body of stand-alone interpreter (to be called in protected mode).
    ** Reads the options and handles them all.
    */
    static int pmain(lua_State* L)
    {
        int    argc = (int)lua_tointeger(L, 1);
        char** argv = (char**)lua_touserdata(L, 2);
        int    script;
        int    args = collectargs(argv, &script);
        char*  progname = nullptr;

        /* luaL_checkversion not available in Lua 5.1/LuaJIT */
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
        luaL_openlibs(L);                      /* open standard libraries */
        createargtable(L, argv, argc, script); /* create table 'arg' */
        /* LUA_GCGEN not available in Lua 5.1/LuaJIT, skip GC configuration */
        if (!(args & has_E)) {               /* no option '-E'? */
            if (handle_luainit(L) != LUA_OK) /* run LUA_INIT */
                return 0;                    /* error running LUA_INIT */
        }
        if (!runargs(L, argv, script)) /* execute arguments -e and -l */
            return 0;                  /* something failed */
        if (script < argc && /* execute main script (if there is one) */
            handle_script(L, argv + script) != LUA_OK)
            return 0;
        if (args & has_i)        /* -i option? */
            doREPL(L, progname); /* do read-eval-print loop */
        else if (script == argc &&
                 !(args & (has_e | has_v))) { /* no arguments? */
            if (lua_stdin_is_tty()) {         /* running in interactive mode? */
                print_version();
                doREPL(L, progname); /* do read-eval-print loop */
            } else
                dofile(L, NULL); /* executes stdin as a file */
        }
        lua_pushboolean(L, 1); /* signal no errors */
        return 1;
    }

    int lua_context::main(int argc, const char** argv)
    {
        int        status, result;
        lua_State* L = m_lua_state;
        if (L == NULL) {
            l_message(argv[0], "cannot create state: not enough memory");
            return EXIT_FAILURE;
        }
        lua_pushcfunction(L, &pmain);   /* to call 'pmain' in protected mode */
        lua_pushinteger(L, argc);       /* 1st argument */
        lua_pushlightuserdata(L, argv); /* 2nd argument */
        status = lua_pcall(L, 2, 1, 0); /* do the call */
        result = lua_toboolean(L, -1);  /* get result */
        report(L, status);
        lua_close(L);
        return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

} // namespace mge::lua