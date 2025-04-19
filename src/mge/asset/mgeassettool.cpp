// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/trace.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

namespace mge {
    MGE_DEFINE_TRACE(ASSSETTOOL);
}

using namespace mge;

int main(int argc, const char** argv)
{
    try {
        if (!mge::configuration::loaded()) {
            mge::configuration::load();
        }

        po::options_description generic("Generic options");
        generic.add_options()("help,h", "Show help message")(
            "version,v",
            "Show version information")("verbose,V", "Enable verbose output");

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
        if (vm.count("verbose")) {
            auto& enable_trace_parameter =
                mge::configuration::find_parameter("trace", "globally_enabled");
            enable_trace_parameter.set_value(true);
            auto& enable_print_to_stdout_parameter =
                mge::configuration::find_parameter("trace", "print_to_stdout");
            enable_print_to_stdout_parameter.set_value(true);
            MGE_INFO_TRACE(ASSSETTOOL)
                << "Verbose output enabled, all trace will be printed to "
                   "stdout";
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