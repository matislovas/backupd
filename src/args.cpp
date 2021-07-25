/**
 * @file args.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <fstream>
#include <string>
#include <iostream>
#include <experimental/filesystem>

#include <boost/program_options.hpp>

#include "args.h"

namespace po = boost::program_options;
namespace fs = std::experimental::filesystem;

po::variables_map args::var_map_;

void args::create_handle(int argc, char *argv[]) {
    po::options_description cmd_opts("Allowed options");

    cmd_opts.add_options()("help,h",    "show this help message");
    cmd_opts.add_options()("version,v", "show version");
    cmd_opts.add_options()("debug,D",   "enable debug logging");
    cmd_opts.add_options()("silent,s",  "silent mode");
    cmd_opts.add_options()("dir,d", po::value<std::string>(), "directory to track");
    cmd_opts.add_options()("bak,b", po::value<std::string>(), "directory to create backups");
    cmd_opts.add_options()("log,l", po::value<std::string>(), "log file path");

    try {
        po::store(
            po::parse_command_line(
                argc,
                argv,
                cmd_opts),
                var_map_);

        po::notify(var_map_);

        if (var_map_.count("help")) {
            std::cout << cmd_opts << std::endl;
            exit(0);
        } else if (var_map_.count("version")) {
            version();
            exit(0);
        }

        if (!flag_enabled("dir") || !flag_enabled("bak")) {
            std::cout 
                << "\'--dir\' and \'--bak\' are required" 
                << std::endl << std::endl;

            std::cout << cmd_opts << std::endl;
            exit(1);
        }

        // Checking if it is not a filepath
        if (!fs::is_directory(fs::path(get<std::string>("dir")))) {
            std::cout 
                << "\'--dir\' must be directory path" 
                << std::endl << std::endl;

            std::cout << cmd_opts << std::endl;
            exit(1);
        }

        // Checking if it is not a filepath
        if (!fs::is_directory(fs::path(get<std::string>("bak")))) {
            std::cout 
                << "\'--bak\' must be directory path" 
                << std::endl << std::endl;

            std::cout << cmd_opts << std::endl;
            exit(1);
        }

        // Sanity check
        if (fs::path(get<std::string>("bak")) == 
            fs::path(get<std::string>("dir"))) {
            std::cout 
                << "\'--bak\' and \'--dir\' paths must be different" 
                << std::endl << std::endl;

            std::cout << cmd_opts << std::endl;
            exit(1);
        }

        if (flag_enabled("log")) {
            fs::file_type log_ftype = fs::status(fs::path(get<std::string>("log"))).type();

            // Log directory cannot be watched
            if (fs::path(get<std::string>("log")) == 
                fs::path(get<std::string>("dir"))) {
                std::cout 
                    << "\'--log\' and \'--dir\' paths must be different" 
                    << std::endl << std::endl;

                std::cout << cmd_opts << std::endl;
                exit(1);
            }

            if (!(log_ftype == fs::file_type::not_found || log_ftype == fs::file_type::regular)) {
                std::cout 
                    << "\'--log\' must be file path: " 
                    << fs::path(get<std::string>("dir")).string() 
                    << std::endl << std::endl;

                std::cout << cmd_opts << std::endl;
                exit(1);
            }
        }
        
        po::notify(var_map_);
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        exit(1);
    }

}

void args::version() {
    std::cout << "backupd version: ";
#if defined(SOFTWARE_MAJOR) && defined(SOFTWARE_MINOR) && defined(SOFTWARE_BUILD)
    std::cout 
        << SOFTWARE_MAJOR << "." 
        << SOFTWARE_MINOR << "."
        << SOFTWARE_BUILD << std::endl;
#else
    std::cout << "unknown" << std::endl;
#endif
}

bool args::flag_enabled(std::string flag) {
    return var_map_.find(flag) != var_map_.end();
}
