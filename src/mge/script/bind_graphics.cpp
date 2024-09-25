// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

#include "mge/graphics/attribute_semantic.hpp"
#include "mge/graphics/buffer_type.hpp"
#include "mge/graphics/context_object.hpp"

namespace mge::script {
    class graphics_script_binder : public script_binder
    {
    public:
        graphics_script_binder() = default;

        void bind() override
        {
            mge::script::module mge("mge");

            mge(type<mge::attribute_semantic>(),
                function("parse_attribute_semantic",
                         &mge::parse_attribute_semantic),
                type<mge::buffer_type>()

                // type<mge::context_object>().method(
                //     "context",
                //     static_cast<render_context& (mge::context_object::*)()>(
                //         &mge::context_object::context))

                //
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_script_binder,
                                mge::script_binder,
                                mgegraphics,
                                graphics);

} // namespace mge::script