#pragma once
#include "pyobject_ref.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

#include "mge/script/module_data.hpp"

#include <string>

namespace mge::python {
    class python_module
    {
    public:
        python_module(python_context&                     context,
                      const mge::script::module_data_ref& data);
        python_module(python_context& context, const std::string& name);

        ~python_module();

    private:
        python_context&              m_context;
        uint64_t                     m_restore_token{0};
        mge::script::module_data_ref m_data;
        std::string                  m_name;
        pyobject_ref                 m_module;
    };
} // namespace mge::python