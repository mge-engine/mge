#include "mge/core/stdexceptions.hpp"
#include "lua_error.hpp"
namespace lua {

    MGE_DEFINE_EXCEPTION(error);

    void
    error::check_status(int status, lua_State *state)
    {
        if(status != LUA_OK) {
            std::string msg(lua_tostring(state, -1));
            lua_pop(state, 1);
            MGE_THROW(lua::error) << msg;
        }
    }

}
