// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"
#include "mge/core/program_options.hpp"
#include "mge/core/properties.hpp"
#include "mge/core/software_component.hpp"
#include "mge/core/trace.hpp"
#include <iostream>

namespace mge {
    MGE_DEFINE_TRACE(ASSETTOOL);
}

MGE_USING_NS_TRACE_TOPIC(mge, ASSETTOOL);

class command
{
public:
    command() = default;
    virtual ~command() = default;

    virtual int execute(const mge::program_options::options& opts) = 0;

    const std::string& name() const
    {
        return m_name;
    }

    const std::string& description() const
    {
        return m_description;
    }

    const mge::program_options& options() const
    {
        return m_options;
    }

protected:
    std::string          m_name;
    std::string          m_description;
    mge::program_options m_options;
};

class info_command : public command
{
public:
    info_command()
    {
        m_name = "info";
        m_description = "Show information about the asset";
        m_options.option("h,help", "Show help message")
            .option("m,mount-point",
                    "Mount asset collections, can be used multiple times",
                    mge::program_options::value<std::string>().composing())
            .option("r", "Mount current directory as root of asset collection")
            .positional("asset",
                        "asset to show information about",
                        mge::program_options::value<std::string>().composing());
    }
    virtual ~info_command() = default;

    int execute(const mge::program_options::options& opts) override
    {
        if (opts.has_option("help") || !opts.has_positional("asset")) {
            std::cout << "usage: mgeassettool info [options] <asset>"
                      << std::endl
                      << std::endl;
            std::cout << m_options << std::endl;
            return 0;
        }

        if (opts.has_option("r")) {
            MGE_DEBUG_TRACE_STREAM(ASSETTOOL)
                << "Mounting current directory as root";
            mge::properties p;
            p.set("directory", ".");
            mge::asset::mount("/", "file", p);
        }

        if (opts.has_option("mount-point")) {
            const auto& mount_points =
                std::any_cast<const std::vector<std::string>&>(
                    opts.option("mount-point"));
            for (const auto& mount_point : mount_points) {
                MGE_DEBUG_TRACE_STREAM(ASSETTOOL)
                    << "Processing mount point: " << mount_point;
            }
        }

        const auto& asset_name = std::any_cast<const std::vector<std::string>&>(
            opts.positional("asset"));
        for (const auto& name : asset_name) {
            try {
                MGE_DEBUG_TRACE_STREAM(ASSETTOOL)
                    << "Processing asset: " << name;
                mge::asset_ref asset = std::make_shared<mge::asset>(name);

                if (asset) {
                    std::cout << "Asset: " << name << std::endl;
                    std::cout << "  Type: " << asset->type() << std::endl;
                    std::cout << "  Size: " << asset->size() << std::endl;
                }

            } catch (const mge::exception& ex) {
                MGE_ERROR_TRACE(ASSETTOOL)
                    << "Error loading asset '" << name << "': " << ex.what();
            }
        }

        return 0;
    }
};

std::vector<std::shared_ptr<command>> commands = {
    std::make_shared<info_command>()};

int main(int argc, const char** argv)
{
    bool is_verbose = false;
    try {
        mge::program_options generic;
        generic.option("h,help", "Show help message")
            .option("v,version", "Show version information")
            .option("V,verbose", "Enable verbose output");

        mge::program_options::options generic_options;
        std::vector<std::string>      generic_args;
        std::string                   command_name;
        std::vector<std::string>      command_args;
        for (int i = 1; i < argc; ++i) {
            if (command_name.empty()) {
                if (std::find_if(
                        commands.begin(),
                        commands.end(),
                        [&argv, i](const std::shared_ptr<command>& cmd) {
                            return cmd->name() == argv[i];
                        }) != commands.end()) {
                    command_name = argv[i];
                } else {
                    generic_args.push_back(argv[i]);
                }
            } else {
                command_args.push_back(argv[i]);
            }
        }

        generic.parse(generic_args, generic_options);

        if (generic_options.has_option("version")) {
            auto mge = mge::software_component::mge();
            std::cout << "mgeassettool version " << mge->version() << " build "
                      << mge->build() << std::endl;
            return 0;
        } else if (generic_options.has_option("help") || argc == 1 ||
                   command_args.empty()) {
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
        if (generic_options.has_option("verbose")) {
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
            MGE_DEBUG_TRACE_STREAM(ASSETTOOL) << "Command: " << cmd->name();
        }
        mge::program_options::options command_options;
        const mge::program_options& command_option_description = cmd->options();
        command_option_description.parse(command_args, command_options);
        return cmd->execute(command_options);

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