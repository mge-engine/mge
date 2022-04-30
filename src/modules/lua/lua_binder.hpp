#pragma once
#include "mge/script/module.hpp"
#include "mge/script/visitor.hpp"
namespace mge::lua {

    class lua_context;

    class lua_binder
    {
    public:
        lua_binder(lua_context& context);
        ~lua_binder() = default;

        void bind(const mge::script::module& m);

        const lua_context& context() const noexcept { return m_context; }
        lua_context&       context() noexcept { return m_context; }

    private:
        lua_context& m_context;
    };
} // namespace mge::lua