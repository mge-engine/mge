// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include <exception>

int main(int argc, char **argv)
{
    try {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (const std::exception &ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught"
                  // boost::current_exception_diagnostic_information()
                  << std::endl;
        return 1;
    }
}
