#pragma once
#include "mge/script/dependency.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
#include "python.hpp"
#include "python_error.hpp"

#include <span>
#include <variant>
#include <vector>

namespace mge::python {

    class bind_step
    {
    public:
        bind_step() {}
        virtual ~bind_step() = default;
        virtual void                    execute() = 0;
        virtual mge::script::dependency provides() const = 0;
    };

    class bind_step_module : public bind_step
    {
    public:
        bind_step_module(const mge::script::module_data_ref& data);
        ~bind_step_module() = default;
        void                    execute() override;
        mge::script::dependency provides() const override;

    private:
        mge::script::module_data_ref m_data;
    };

    class bind_step_function : public bind_step
    {
    public:
        bind_step_function(const mge::script::module_data_ref&   module,
                           const mge::script::function_data_ref& data);
        ~bind_step_function() = default;
        void                    execute() override;
        mge::script::dependency provides() const override;

    private:
        mge::script::module_data_ref   m_module;
        mge::script::function_data_ref m_data;
    };

    class bind_step_type : public bind_step
    {
    public:
        bind_step_type(const mge::script::module_data_ref& module,
                       const mge::script::type_data_ref&   data);
        ~bind_step_type() = default;
        void                    execute() override;
        mge::script::dependency provides() const override;

    private:
        mge::script::module_data_ref m_module;
        mge::script::type_data_ref   m_data;
    };

    MGE_DECLARE_REF(bind_step);
} // namespace mge::python