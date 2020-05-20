// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"
#include "mge/core/log.hpp"
#include "mge/core/module.hpp"
#include "test/googletest.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#endif
#include <boost/exception/diagnostic_information.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5204)
#endif
#include <exception>

MGE_DEFINE_LOG(TEST);

int main(int argc, char **argv)
{
    try {
        MGE_DEBUG_LOG(TEST) << "Run test " << argv[0];
        mge::module::load_all();
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << boost::current_exception_diagnostic_information()
                  << std::endl;
        return 1;
    }
}
