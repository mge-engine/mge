// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/visitor.hpp"
#include <map>
#include <typeindex>

namespace mge::lua {

    class lua_context;

    class lua_bind_helper : public mge::script::visitor
    {
    public:
        using mge::script::visitor::visit;

        lua_bind_helper(lua_context& context);
        virtual ~lua_bind_helper();

        void begin(const mge::script::module_details& m) override;
        void end(const mge::script::module_details& m) override;

        void begin(const mge::script::type_details& t) override;
        void end(const mge::script::type_details& t) override;

        void visit(const mge::script::function_details& f) override;
        void visit(const mge::script::variable_details& v) override;

        void bind(const mge::script::module& m);

        void enum_value(const std::string&              name,
                        const mge::script::any_integer& value) override;

    private:
        lua_context&                                             m_context;
        std::map<std::type_index, mge::script::type_details_ref> m_pod_types;
    };

} // namespace mge::lua