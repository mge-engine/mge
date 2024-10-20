#pragma once
#include "lua_scope.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::lua {

    class module : public scope
    {
    public:
        module(lua_context& context, const char* name);
        module(lua_context& context, const mge::script::module_data_ref& m);
        module(lua_context&                        context,
               const scope_ref&                    parent,
               const mge::script::module_data_ref& m);
        virtual ~module() = default;

    private:
        mge::script::module_data_ref m_module;
    };

} // namespace mge::lua