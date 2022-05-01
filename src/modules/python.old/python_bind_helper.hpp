// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/visitor.hpp"
#include "python_module.hpp"
#include "python_type.hpp"
#include <stack>
#include <typeindex>

namespace mge::python {

    class python_context;

    class python_bind_helper : public mge::script::visitor
    {
    public:
        using mge::script::visitor::visit;

        python_bind_helper(python_context& context);
        virtual ~python_bind_helper();

        void begin(const mge::script::module_details& m) override;
        void end(const mge::script::module_details& m) override;

        void begin(const mge::script::type_details& t) override;
        void end(const mge::script::type_details& t) override;

        void visit(const mge::script::function_details& f) override;
        void visit(const mge::script::variable_details& v) override;

        void bind(const mge::script::module& m);

        void enum_value(const std::string&              name,
                        const mge::script::any_integer& value) override;
        void field(const mge::script::field_details& field) override;
        void constructor(const mge::script::signature&        s,
                                 const mge::script::context_function& cf) override;

    private:
        void begin_class(const mge::script::type_details& t);
        void end_class(const mge::script::type_details& t);
        void begin_enum(const mge::script::type_details& t);
        void end_enum(const mge::script::type_details& t);
        void define_function_type(python_module_ref& mge_module);
        void bind_basic_modules();

        python_context&               m_context;
        std::stack<python_module_ref> m_module_stack;
        python_type_ref               m_current_type;
        python_complex_type_ref       m_current_complex_type;
        std::map<std::type_index, mge::script::type_details_ref> m_pod_types;
    };

} // namespace mge::python