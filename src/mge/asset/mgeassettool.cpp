// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, const char** argv)
{
    try {
        po::options_description desc("Generic options");
        desc.add_options()("help,h",
                           "Show help message")("version,v",
                                                "Show version information");
        po::variables_map vm;
        auto              parsed = po::parse_command_line(argc, argv, desc);
        po::store(parsed, vm);
        po::notify(vm);

        if (vm.count("version")) {
            std::cout << "mgeassettool version <<unknown>>" << std::endl;
            return 0;
        } else if (vm.count("help") || argc == 1) {
            std::cout << "usage: mgeassettool [options] [<command> [command "
                         "options] [command arguments]]"
                      << std::endl
                      << std::endl;
            std::cout << desc << std::endl;
            return 0;
        }

        std::cout << "Not yet implemented: mgeassettool" << std::endl;

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return 1;
    }
    return 0;
}