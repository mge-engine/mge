// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_context.hpp"
#include "mge/script/script_engine.hpp"
#include "mge/script/script_fwd.hpp"

#include <iostream>

int main(int argc, const char** argv)
{
    try {
        if (!mge::configuration::loaded()) {
            mge::configuration::load();
        }
        mge::module::load_all();
        mge::script_engine_ref  engine = mge::script_engine::create("lua");
        mge::script_context_ref context = engine->create_context();
        mge::script::module     global_module;
        context->bind();
        return context->main(argc, argv);
    } catch (const mge::exception& mge_ex) {
        std::cerr << mge_ex.details();
    } catch (const std::exception& std_ex) {
        std::cerr << "Exception occurred: " << std_ex.what();
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }

    return 1;
}