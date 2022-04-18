// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "python.hpp"

namespace mge::python {

    MGE_DECLARE_REF(python_type);

    class python_context;

    class python_type
    {
    public:
        python_type(python_context&                      context,
                    const mge::script::type_details_ref& type);
        virtual ~python_type() = default;

    private:
        python_context&               m_context;
        mge::script::type_details_ref m_type;
    };
} // namespace mge::python