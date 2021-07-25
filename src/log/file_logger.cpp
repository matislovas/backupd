/**
 * @file file_logger.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <chrono>
#include <cstdio>
#include <iomanip>

#include <errno.h>
#include <iostream>

#include "log/file_logger.h"


namespace Log {

namespace time = std::chrono;

void file_logger::log(const entry& block) {
    auto now = time::system_clock::to_time_t(time::system_clock::now());

    std::unique_lock<std::mutex> lock(mutex);

	this->file << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << ": ";
    this->file << "[" << Flags::flag_to_string(block.filter_flag) << "] ";
    this->file << "[" << block.prefix << "] ";
	this->file << "   " << block.message;
	this->file << std::endl;
}

file_logger::file_logger(const fs::path& path) {
    if (fs::exists(path)) {
        fs::remove(path);
    }

    this->file.open(
        path.c_str(), 
        std::ofstream::out |
        std::ofstream::app);

    if (!this->file.is_open()) {
        throw std::runtime_error(
            std::string("Failed to create log file: ") + fs::absolute(path).string()); 
    }
}

file_logger::~file_logger() {
    this->file.close();
}

} //  namespace Log
