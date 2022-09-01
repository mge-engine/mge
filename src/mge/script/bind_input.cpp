// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class input_script_binder : public script_binder
    {
    public:
        input_script_binder() = default;

        void bind()
        {
            module("mge")(type<mge::key>("key")
                          // module end
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(input_script_binder,
                                mge::script_binder,
                                mgeinput,
                                input);

} // namespace mge::script