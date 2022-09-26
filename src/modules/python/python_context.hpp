// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_context.hpp"
#include "python.hpp"
#include "python_fwd.hpp"
#include "python_module.hpp"
#include "python_type.hpp"

#include <unordered_map>

namespace mge::python {
    class python_context : public script_context
    {
    public:
        python_context(const python_engine_ref& engine);
        virtual ~python_context();

        void eval(const std::string& code) override;
        void bind(const mge::script::module& m) override;
        int  main(int argc, const char** argv) override;

        python_module_ref get_or_add_module(const mge::script::module& m);
        python_module_ref get_module(const mge::script::module& m) const;

        void interpreter_lost();
        void rebuild() const;

    private:
        python_engine_ref                          m_engine;
        mutable std::vector<std::function<void()>> m_rebind_functions;

        std::unordered_map<mge::script::module, python_module_ref>
            m_python_modules;
        std::unordered_map<mge::script::type_details_ref, python_type_ref>
            m_python_types;

        mutable bool m_needs_rebuild;
    };
} // namespace mge::python