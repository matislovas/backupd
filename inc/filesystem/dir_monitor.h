/**
 * @file dir_monitor.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_DIR_MONITOR_H_
#define INC_DIR_MONITOR_H_

#include <vector>
#include <utility>
#include <memory>
#include <array>

#include <boost/asio.hpp>

#include "filesystem/event.h"
#include "log/logger.h"

#define INPUT_BUFF_SIZE 4096

namespace Filesystem {

typedef std::pair<event_mask, event_handler> watch;

class dir_monitor {
  public:
    dir_monitor(
        boost::asio::io_service& io_,
        Log::logger& logger_,
        fs::path dir_path_);

    dir_monitor() = delete;
    ~dir_monitor();

    /**
     * Add event watch to track from all inotify's events
     *
     * @param em        Inotify's event mask to track
	 * @param handler   Handler callback func
     *
     */
    void add_watch(event_mask em, event_handler handler);


  private:
    /**
     * Inotify event parser and notifier
     *
     * @param err       Err code placeholder
	 * @param rx_size   Bytes transfered from file descriptor
     *
     */
    void dir_event_handler(
        const boost::system::error_code& err,
        size_t rx_size);

    /**
     * Starts event tracking
     *
     */
    void start_tracking_events();


    int file_descr;
    int watch_descr;
    int error;

    fs::path dir_path;

    boost::asio::io_service& io;
    Log::logger& logger;

    std::array<char, INPUT_BUFF_SIZE> s_buff;
    std::shared_ptr<boost::asio::posix::stream_descriptor> s_descr;


    std::vector<watch> watches;
};

}  // namespace Filesystem

#endif  // INC_DIR_MONITOR_H_
