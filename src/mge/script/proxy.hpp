// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

namespace mge::script {

    class invocation_context;

    template <typename T> class proxy : public T
    {
    public:
        proxy() = default;
        virtual ~proxy() = default;

        void set_context(invocation_context* context) { m_context = context; }

    private:
        invocation_context* m_context;
    };

} // namespace mge::script