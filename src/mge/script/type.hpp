// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string_view>

namespace mge::script {

    class MGESCRIPT_EXPORT type
    {
    protected:
        type(const std::string_view name);

    public:
        virtual ~type();
        std::string_view name() const;

    private:
        std::string m_name;
    };

} // namespace mge::script