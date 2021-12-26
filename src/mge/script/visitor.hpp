// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/any_integer.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    /**
     * Vistor of reflection infrastructure.
     */
    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;

        void visit(const module& m);

        virtual void begin(const module_details& m);
        virtual void end(const module_details& m);

        virtual void begin(const type_details& t);
        virtual void end(const type_details& t);

        virtual void visit(const function_details& f);
        virtual void visit(const variable_details& v);

        virtual void enum_value(const std::string& name,
                                const any_integer& value);

        virtual void field(const field_details& field);
        virtual void constructor(const signature&        signature,
                                 const context_function& cf);
    };
} // namespace mge::script