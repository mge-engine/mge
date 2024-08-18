// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"

namespace mge::script {

    class MGESCRIPT_EXPORT module
    {
    public:
        /**
         * Root module.
         */
        module();

        /**
         * Destructor.
         */
        ~module();

        mge::script::module& operator=(const mge::script::module&) = delete;
    };

} // namespace mge::script