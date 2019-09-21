// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/core/log.hpp"
#include "mge/core/module.hpp"
#include "mge/core/exception.hpp"
#include <exception>
#include <boost/exception/diagnostic_information.hpp>

MGE_DEFINE_LOG(BENCHMARK);

int main(int argc, char** argv)
{
    try {
        MGE_DEBUG_LOG(BENCHMARK) << "Run benchmark " << argv[0];
        mge::module::load_all();
        ::benchmark::Initialize(&argc, argv);
        if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
            return 1;
        }
        ::benchmark::RunSpecifiedBenchmarks();

        return 0;
    } catch(const std::exception& ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        return 1;
    }
}
