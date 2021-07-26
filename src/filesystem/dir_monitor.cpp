/**
 * @file dir_monitor.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <iostream>

#include <sys/inotify.h>

#include <boost/bind.hpp>

#include "filesystem/dir_monitor.h"

namespace Filesystem {

// This class is for monitoring one dir only, but since we're doing here exactly it, it is ok for now
dir_monitor::dir_monitor(
    boost::asio::io_service& io_,
    Log::logger& logger_,
    fs::path dir_path_):
    file_descr(0),
    watch_descr(0),
    error(0),
    dir_path(dir_path_),
    io(io_),
    logger(logger_) {
    std::stringstream err_str;
    
    if (!fs::exists(dir_path)) {
        this->error = errno;
        err_str << "Directory does not exist " 
                << strerror(this->error);
        throw std::runtime_error(err_str.str());
    }
    
    if (!fs::is_directory(dir_path)) {
        this->error = errno;
        err_str << "Path provided is not a directory " 
                << strerror(this->error);
        throw std::runtime_error(err_str.str());
    }

    // Trying to initialise inotify service
    this->file_descr = inotify_init1(IN_NONBLOCK);

    if (this->file_descr == -1) {
        this->error = errno;
        err_str << "inotify_init1 failed " 
                << strerror(this->error);
        throw std::runtime_error(err_str.str());
    }

    this->s_descr.reset(
        new boost::asio::posix::stream_descriptor(
            this->io,
            this->file_descr));

    // Adding dir to watch list
    this->watch_descr = inotify_add_watch(
        this->file_descr,
        dir_path.c_str(),
        to_event_mask(
            event_type::all_events));

    if (this->watch_descr == -1) {
        this->error = errno;
        err_str << "Inotify error " 
                << strerror(this->error)
                << " for path " 
                << dir_path.string();
        throw std::runtime_error(err_str.str());
    }

    start_tracking_events();
}

dir_monitor::~dir_monitor() {
    // TODO this could theoretically throw errors ... need to handle?
    inotify_rm_watch(this->file_descr, this->watch_descr);
    this->s_descr->close();
}

void dir_monitor::add_watch(event_mask em, event_handler handler) {
    if (handler) {
        // TODO do we permit multiple handlers on the same mask? 
        // Or should we use std::map here ..?
        this->watches.push_back(watch(em, handler));
    }
}

void dir_monitor::start_tracking_events() {
    this->s_descr->async_read_some(
        boost::asio::buffer(this->s_buff, INPUT_BUFF_SIZE),
        boost::bind(
            &dir_monitor::dir_event_handler,
            this,
            _1,
            _2));
}

void dir_monitor::dir_event_handler(
    const boost::system::error_code& err,
    size_t rx_size) {
    if (err) {
        return;
    }
    
    std::vector<char> rx(
            this->s_buff.data(),
            this->s_buff.data() + rx_size);

    while(rx.size() >= sizeof(inotify_event)) {
        const inotify_event* i_event = reinterpret_cast<const inotify_event*>(
            rx.data());

        size_t rx_size = sizeof(inotify_event) + i_event->len;

        for (auto & watch : this->watches) {
            if (watch.first & i_event->mask) {
                std::string filepath = this->dir_path.string() + std::string(i_event->name);

                this->io.post(
                    boost::bind(
                        watch.second,
                        err,
                        event(
                            i_event->mask,
                            std::move(filepath),
                            std_clock::now())));
            }
        }

        rx.erase(
            rx.begin(),
            rx.begin() + rx_size);
    }

    this->start_tracking_events();
}

}  // namespace Filesystem
