#include "lua_bind_helper.hpp"
#include "mge/script/type_details.hpp"

namespace mge::lua {
    lua_bind_helper::lua_bind_helper(lua_context& context)
        : m_context(context)
    {}

    lua_bind_helper::~lua_bind_helper() {}

    void lua_bind_helper::bind(const mge::script::module& m) { visit(m); }

    void lua_bind_helper::begin(const mge::script::module_details& m) {}

    void lua_bind_helper::end(const mge::script::module_details& m) {}

    void lua_bind_helper::begin(const mge::script::type_details& t)
    {
        if (t.type_class().is_pod() || t.type_class().is_void) {
            mge::script::type_details_ref t_ref =
                const_cast<mge::script::type_details&>(t).shared_from_this();
            m_pod_types[t.type_index()] = t_ref;
        } else if (t.type_class().is_enum) {
            return;
        }
    }

    void lua_bind_helper::enum_value(const std::string&              name,
                                     const mge::script::any_integer& value)
    {
        return;
    }

    void lua_bind_helper::end(const mge::script::type_details& t) {}

    void lua_bind_helper::visit(const mge::script::function_details& v) {}
    void lua_bind_helper::visit(const mge::script::variable_details& v) {}

} // namespace mge::lua