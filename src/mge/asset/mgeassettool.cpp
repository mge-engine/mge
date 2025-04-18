// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, const char** argv)
{
    try {
        po::options_description generic("Generic options");
        generic.add_options()("help,h",
                              "Show help message")("version,v",
                                                   "Show version information");

        std::vector<std::string> args(argv + 1, argv + argc);
        po::variables_map        vm;
        po::parsed_options       parsed =
            po::command_line_parser(args)
                .options(generic)
                .allow_unregistered() // Allow subcommand and its args to pass
                                      // through
                .run();
        po::store(parsed, vm);
        po::notify(vm);

        std::vector<std::string> remaining =
            po::collect_unrecognized(parsed.options, po::include_positional);

        if (vm.count("version")) {
            std::cout << "mgeassettool version <<unknown>>" << std::endl;
            return 0;
        } else if (vm.count("help") || argc == 1 || remaining.empty()) {
            std::cout << "usage: mgeassettool [options] [<command> [command "
                         "options] [command arguments]]"
                      << std::endl
                      << std::endl;
            std::cout << generic << std::endl;
            return 0;
        }
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