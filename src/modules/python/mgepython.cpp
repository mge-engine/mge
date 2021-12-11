// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/bind.hpp"
#include "mge/script/script_context.hpp"
#include "mge/script/script_engine.hpp"
#include "mge/script/script_fwd.hpp"

#include <iostream>

int main(int argc, const char** argv)
{

    if (!mge::configuration::loaded()) {
        mge::configuration::load();
    }
    mge::module::load_all();
    mge::script_engine_ref  engine = mge::script_engine::create("python");
    mge::script_context_ref context = engine->create_context();

    mge::script::module global_module;
    context->bind(global_module);
    return context->main(argc, argv);
}