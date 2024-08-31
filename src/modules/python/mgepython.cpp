// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_context.hpp"
#include "mge/script/script_engine.hpp"


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
        MGE_DEBUG_TRACE(MGEPYTHON) << "Binding types";
        //
        int rc = context->main(argc, argv);
        MGE_DEBUG_TRACE(MGEPYTHON) << "Python main returned: " << rc;
        MGE_DEBUG_TRACE(MGEPYTHON) << "Releasing context";
        context.reset();
        MGE_DEBUG_TRACE(MGEPYTHON) << "Context released";
        return rc;
    } catch (const mge::exception& e) {
        MGE_ERROR_TRACE(MGEPYTHON) << "Caught exception: " << e.details();
        std::cerr << e.details();
    } catch (const std::exception& e) {
        MGE_ERROR_TRACE(MGEPYTHON) << "Caught exception: " << e.what();
        std::cerr << "Caught exception: " << e.what();
    }
    return 1;
}
