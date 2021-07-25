/**
 * @file main.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "args.h"
#include "log/log.h"
#include "filesystem/dir_monitor.h"
#include "filesystem/backup_handler.h"


#define LOG_PREFIX "main"


int main(int ac, char* av[]) {
    int return_code = 1;
    boost::asio::io_service io;

    // Handle Posix signals
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);

    signals.async_wait(
        boost::bind(
            &boost::asio::io_service::stop, &io));

    args::create_handle(ac, av);

    Log::filter filters;

    filters.set( 
        Log::Flags::EVENT | 
        Log::Flags::ERROR | 
        Log::Flags::WARNING);

    if (args::flag_enabled("debug")) {
        filters.set(Log::Flags::DEBUG);
    }

    Log::logger logger(filters);

    if (!args::flag_enabled("silent")) {
        logger.add_output(Log::console_logger::create());
    }

    if (args::flag_enabled("log")) {
        try {
            logger.add_output(
                Log::file_logger::create(
                    fs::absolute(fs::path(args::get<std::string>("log")))));
        } catch (std::exception& err) {
            int return_code = 2;
            
            LOG(
                logger,
                Log::Flags::ERROR,
                "Error: %s",
                err.what());

            return return_code;
        }
    }

    LOG(
        logger,
        Log::Flags::DEBUG,
        "Starting ...");

    try {
        Filesystem::dir_monitor monitor(
            io,
            logger,
            fs::absolute(fs::path(args::get<std::string>("dir"))));

        Filesystem::backup_handler bckp_handler(
            io,
            logger,
            fs::absolute(fs::path(args::get<std::string>("dir"))),
            fs::absolute(fs::path(args::get<std::string>("bak"))));

        Filesystem::event_mask ev_mask = Filesystem::to_event_mask(
            Filesystem::event_type::create |
            Filesystem::event_type::modify |
            Filesystem::event_type::moved_to);

        monitor.add_watch(
            ev_mask, 
            boost::bind(
                &Filesystem::backup_handler::event_interface,
                &bckp_handler,
                _1,
                _2));

        io.run();

        return_code = 0;
    } catch (std::exception& err) {
        LOG(
            logger,
            Log::Flags::ERROR,
            "Error: %s",
            err.what());
    }

    io.stop();

    return return_code;
}
