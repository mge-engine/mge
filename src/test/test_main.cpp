// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/core/log.hpp"
#include "mge/core/module.hpp"
#include "mge/core/exception.hpp"
#include <exception>
MGE_DEFINE_LOG(TEST);

static void unexpected_handler_function(void)
{
    std::cerr << "Unexpected exception caught" << std::endl;
    std::abort();
}

static void terminate_handler_function(void)
{
    std::cerr << "Terminate handler called due to uncatched exception" << std::endl;
    if(mge::exception::current_exception()) {
        std::cerr << mge::exception::current_exception()->details() << std::endl;
    } else {
        std::cerr << "No current exception" << std::endl;
    }
    std::abort();
}


int main(int argc, char **argv)
{
    try {
        std::set_unexpected(unexpected_handler_function);
        std::set_terminate(terminate_handler_function);
        MGE_DEBUG_LOG(TEST) << "Run test " << argv[0];
        mge::module::load_all();
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch(const std::exception& ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return 1;
    }
}
