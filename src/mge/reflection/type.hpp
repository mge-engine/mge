// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/types.hpp"
#include "mge/reflection/reflection_fwd.hpp"

#include <string>

namespace mge {
    namespace reflection {

        /**
         * A type.
         */
        class MGE_REFLECTION_EXPORT type
        {
        public:
            template <typename T>
            static type& get();

            type(const std::string& name);
            ~type() = default;
        private:
            std::string m_name;
        };
}
}
