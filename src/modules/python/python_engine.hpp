#pragma once
#include "mge/script/script_engine.hpp"
#include "python.hpp"
namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        mge::script_context_ref create_context() override;
    };
} // namespace mge::python