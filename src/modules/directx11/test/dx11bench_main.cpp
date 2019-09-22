
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "bench/googlebenchmark.hpp"
#include "mge/application/application.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/component.hpp"

using mge::application;

class dx11bench_application : public application
{
public:
    dx11bench_application()
    {}

    ~dx11bench_application()
    {}

    void start() override
    {
        int argc = this->argc();
        char **argv = (char **) this->argv();
        try {
            ::benchmark::Initialize(&argc, argv);
            if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
                set_return_code(1);
            } else {
                ::benchmark::RunSpecifiedBenchmarks();
            }
        } catch(const std::exception& ex) {
            std::cerr << "Exception caught: " << ex.what() << std::endl;
            set_return_code(1);
        }

        if(!is_quit()) {
            set_quit();
        }
    }
};

MGE_REGISTER_IMPLEMENTATION(dx11bench_application,
                            application, dx11bench_application);

MGE_MAINFUNCTION


