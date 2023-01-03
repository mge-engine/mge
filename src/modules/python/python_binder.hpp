// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/visitor.hpp"

#include "python_type.hpp"

namespace mge::python {
    class python_context;

    class python_binder
    {
    public:
        python_binder(python_context& context);
        ~python_binder() = default;

        void bind(const mge::script::module& m);

        python_context&       context() { return m_context; }
        const python_context& context() const { return m_context; }

        void add_type(std::type_index i, const python_type_ref& t)
        {
            m_created_types.emplace(i, t);
        }

        const python_type_ref get_type(std::type_index i) const
        {
            auto it = m_created_types.find(i);
            if (it == m_created_types.end()) {
                MGE_THROW(mge::no_such_element) << "No type for " << i.name();
            }
            return it->second;
        }

        void add_init_code(const std::string& s);
    private:
        void prologue();
        void init();

        python_context&                            m_context;
        std::map<std::type_index, python_type_ref> m_created_types;
        std::vector<std::string>                   m_init_code;
    };

} // namespace mge::python