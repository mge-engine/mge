// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>

namespace mge::script {

    /**
     * @brief Visit a module.
     */
    class MGESCRIPT_EXPORT visitor
    {
    protected:
        visitor() = default;

    public:
        virtual ~visitor() = default;

        virtual void start(const module_details& m);
        virtual void finish(const module_details& m);
        virtual void start(const type_details& t);
        virtual void finish(const type_details& t);
        virtual void enum_value(const std::string& name, int64_t value);
    };

} // namespace mge::script