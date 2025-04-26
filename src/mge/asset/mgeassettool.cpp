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

class command
{
public:
    command() = default;
    virtual ~command() = default;

    virtual int execute(const po::variables_map& vm) = 0;

    const std::string& name() const
    {
        return m_name;
    }
    const std::string& description() const
    {
        return m_description;
    }
    const po::options_description& options() const
    {
        return m_options;
    }

protected:
    std::string             m_name;
    std::string             m_description;
    po::options_description m_options;
};

class info_command : public command
{
public:
    info_command()
    {
        // clang-format off
        m_name = "info";
        m_description = "Show information about the asset";
        m_options.add_options()
            ("help,h", "Show help message")
            ("mount-point,m", po::value<std::vector<std::string>>()->composing(),  "mount asset collections, can be used multople times")
            ("asset", po::value<std::vector<std::string>>(), "asset to show information about");
        // clang-format on
    }
    virtual ~info_command() = default;

    int execute(const po::variables_map& vm) override
    {
        if (vm.count("help")) {
            std::cout << "usage: mgeassettool info [options] <asset>"
                      << std::endl
                      << std::endl;
            std::cout << m_options << std::endl;
            return 0;
        }
        return 1;
    }
};

std::vector<std::shared_ptr<command>> commands = {
    std::make_shared<info_command>()};

int main(int argc, const char** argv)
{
    bool is_verbose = false;
    try {
        po::options_description generic("Generic options");
        generic.add_options()("help,h", "Show help message")(
            "version,v",
            "Show version information")("verbose,V", "Enable verbose output");

        std::vector<std::string> command_args;
        std::vector<std::string> args;
        std::string              command_name;
        bool                     found_command = false;
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (found_command) {
                command_args.push_back(arg);
            } else if (!arg.empty() && arg[0] == '-') {
                args.push_back(arg);
            } else {
                found_command = true;
                command_name = arg;
            }
        }

        po::variables_map  vm;
        po::parsed_options parsed =
            po::command_line_parser(args).options(generic).run();
        po::store(parsed, vm);
        po::notify(vm);

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
            std::cout << generic << std::endl << std::endl;
            std::cout << "Available commands:" << std::endl;
            for (const auto& cmd : commands) {
                std::cout << "  " << cmd->name() << "\t\t" << cmd->description()
                          << std::endl;
            }
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

        const auto it =
            std::find_if(commands.begin(),
                         commands.end(),
                         [&command_name](const std::shared_ptr<command>& cmd) {
                             return cmd->name() == command_name;
                         });

        if (it == commands.end()) {
            std::cerr << "mgeassettool: '" << command_name
                      << "' is not a valid command. See 'mgeassettool --help'."
                      << std::endl;
            return 1;
        }
        auto& cmd = *it;

        if (is_verbose) {
            MGE_DEBUG_TRACE(ASSETTOOL) << "Command: " << cmd->name();
        }

        {
            po::options_description options = cmd->options();
            po::variables_map       vm_command;
            po::store(
                po::command_line_parser(command_args).options(options).run(),
                vm_command);
            po::notify(vm_command);
            return cmd->execute(vm_command);
        }
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