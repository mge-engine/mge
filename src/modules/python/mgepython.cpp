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

MGE_DEFINE_TRACE(MGEPYTHON);

int main(int argc, const char** argv)
{
    try {
        if (!mge::configuration::loaded()) {
            mge::configuration::load();
        }
        mge::module::load_all();
        MGE_DEBUG_TRACE(MGEPYTHON) << "Creating python engine";
        mge::script_engine_ref engine = mge::script_engine::create("python");
        MGE_DEBUG_TRACE(MGEPYTHON) << "Creating python context";
        mge::script_context_ref context = engine->create_context();
        MGE_DEBUG_TRACE(MGEPYTHON) << "Binding global module";
        mge::script::module global_module;
        context->bind(global_module);
        MGE_DEBUG_TRACE(MGEPYTHON) << "Run main";
        int rc = context->main(argc, argv);
        MGE_DEBUG_TRACE(MGEPYTHON) << "Python main returned: " << rc;
        MGE_DEBUG_TRACE(MGEPYTHON) << "Releasing context";
        context.reset();
        MGE_DEBUG_TRACE(MGEPYTHON) << "Context released";
        return rc;
    } catch (const mge::exception& mge_ex) {
        MGE_ERROR_TRACE(MGEPYTHON)
            << "Caught mge exception: " << mge_ex.details();
        std::cerr << mge_ex.details();
    } catch (const std::exception& std_ex) {
        MGE_ERROR_TRACE(MGEPYTHON) << "Caught exception: " << std_ex.what();
        std::cerr << "Caught exception: " << std_ex.what();
    }
    return 1;
}