/**
 * @file event.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_FILESYSTEM_EVENT_H_
#define INC_FILESYSTEM_EVENT_H_

#include <functional>
#include <chrono>
#include <experimental/filesystem>

#include <boost/system/error_code.hpp>

#include "filesystem/event_type.h"

namespace fs = std::experimental::filesystem;

namespace Filesystem {

typedef std::chrono::steady_clock std_clock;
typedef std::chrono::system_clock sys_clock;

struct event {
 public:
    event(
        const event_mask type_,
        const std::string filepath_,
        std_clock::time_point time_):
        type(type_),
        filepath(filepath_),
        time(time_) {}

    event_mask type;
    fs::path filepath;
    std_clock::time_point time;
};

typedef std::function<void(const boost::system::error_code&, event)> event_handler;

}  // namespace Filesystem

#endif  // INC_FILESYSTEM_EVENT_TYPE_H_
