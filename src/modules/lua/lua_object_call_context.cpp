#include "lua_object_call_context.hpp"
#include "lua_type.hpp"

#include "mge/core/checked_cast.hpp"

namespace mge::lua {

    void* lua_object_call_context::this_ptr()
    {
        std::shared_ptr<int>* sptr =
            reinterpret_cast<std::shared_ptr<int>*>(m_userdata);
        if (sptr) {
            return sptr->get();
        } else {
            return nullptr;
        }
    }

    void* lua_object_call_context::shared_ptr_address() { return m_userdata; }

    bool lua_object_call_context::bool_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return lua_toboolean(m_lua_state, index + 1) != 0;
    }

    int8_t lua_object_call_context::int8_t_parameter(size_t position)
    {
        int index = static_cast<int>(position);
        return checked_cast<int8_t>(lua_tointeger(m_lua_state, index + 1));
    }

} // namespace mge::lua