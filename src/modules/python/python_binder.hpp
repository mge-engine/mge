// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/type_details.hpp"
#include "mge/reflection/visitor.hpp"
#include "mge/script/script_context.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

namespace mge::python {

    class python_context;

    class python_binder : public mge::reflection::visitor
    {
    public:
        python_binder(python_context& context)
            : m_context(context)
        {}

        ~python_binder() override;

        void before(const mge::reflection::module_details& details) override;
        void on(const mge::reflection::module_details& details) override;
        void after(const mge::reflection::module_details& details) override;

        void before(const mge::reflection::type_details& details) override;
        void on(const mge::reflection::type_details& details) override;
        void after(const mge::reflection::type_details& details) override;

        void before(const mge::reflection::function_details& details) override;
        void on(const mge::reflection::function_details& details) override;
        void after(const mge::reflection::function_details& details) override;

    private:
        void bind_enum(const mge::reflection::type_details& details);

        python_context&                m_context;
        std::vector<python_module_ref> m_module_stack;
    };

} // namespace mge::python
