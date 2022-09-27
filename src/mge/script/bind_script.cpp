// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"
#include "mge/script/type_details.hpp"

namespace mge::script {

    class script_script_binder : public script_binder
    {
    public:
        script_script_binder() = default;
        void bind()
        {
            module("mge")(module("script")(
                type<module_details>("module_details")
                    .constructor()
                    .constructor<const std::string&>()
                    .method("get", &module_details::get)
                    .method("is_root", &module_details::is_root)
                    .method("parent", &module_details::parent)
                    .method("name", &module_details::name)
                    .method("full_name", &module_details::full_name),
                type<type_details>("type_details"),
                type<signature>("signature")));
        }
    };
    MGE_REGISTER_IMPLEMENTATION(script_script_binder,
                                mge::script_binder,
                                mgescript,
                                script);

} // namespace mge::script