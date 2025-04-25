// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/software_component.hpp"
#include "mge/core/trace.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

namespace mge {
    MGE_DEFINE_TRACE(ASSETTOOL);
}

MGE_USING_NS_TRACE_TOPIC(mge, ASSETTOOL);

int main(int argc, const char** argv)
{
    bool is_verbose = false;
    try {
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

        std::vector<std::string> command_args =
            po::collect_unrecognized(parsed.options, po::include_positional);

        if (vm.count("version")) {
            auto mge = mge::software_component::mge();
            std::cout << "mgeassettool version " << mge->version() << " build "
                      << mge->build() << std::endl;
            return 0;
        } else if (vm.count("help") || argc == 1 || command_args.empty()) {
            std::cout << "usage: mgeassettool [options] [<command> [command "
                         "options] [command arguments]]"
                      << std::endl
                      << std::endl;
            std::cout << generic << std::endl;
            return 0;
        }
        if (vm.count("verbose")) {
            is_verbose = true;
            auto& enable_trace_parameter =
                mge::configuration::find_parameter("trace", "globally_enabled");
            enable_trace_parameter.set_value(true);
            auto& enable_print_to_stdout_parameter =
                mge::configuration::find_parameter("trace", "print_to_stdout");
            enable_print_to_stdout_parameter.set_value(true);
            if (!mge::configuration::loaded()) {
                mge::configuration::load();
            }
            MGE_INFO_TRACE(ASSETTOOL)
                << "Verbose output enabled, all trace will be printed to "
                   "stdout";
            mge::module::load_all();
        } else {
            if (!mge::configuration::loaded()) {
                mge::configuration::load();
            }
            mge::module::load_all();
        }

        std::string command = command_args[0];
        command_args.erase(command_args.begin());
        if (!command.empty()) {
            std::cerr << "mgeassettool: '" << command
                      << "' is not a valid command. See 'mgeassettool --help'."
                      << std::endl;
            return 1;
        }
        if (is_verbose) {
            MGE_DEBUG_TRACE(ASSETTOOL) << "Command: " << command;
        }

        return 0;
    } catch (const mge::exception& ex) {
        if (!is_verbose) {
            std::cerr << "Error: " << ex.what() << std::endl;
        } else {
            MGE_ERROR_TRACE(ASSETTOOL) << "Error: " << ex;
        }
        return 1;
    } catch (const std::exception& ex) {
        if (!is_verbose) {
            std::cerr << "Error: " << ex.what() << std::endl;
        } else {
            MGE_ERROR_TRACE(ASSETTOOL) << "Error: " << ex.what();
        }
        return 1;
    } catch (...) {
        if (!is_verbose) {
            std::cerr << "Unknown error" << std::endl;
        } else {
            MGE_ERROR_TRACE(ASSETTOOL) << "Unknown error";
        }
        return 1;
    }
    return 0;
}