/**
 * @file console_logger.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "log/console_logger.h"
#include "log/flags.h"


namespace Log {

namespace time = std::chrono;

void console_logger::log(const entry& block) {
    std::ostringstream stream;

	auto now = time::system_clock::to_time_t(time::system_clock::now());

	stream << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << ": ";
    stream << "[" << Flags::flag_to_string(block.filter_flag) << "] ";
    stream << "[" << block.prefix << "] ";
	stream << "   " << block.message;

	std::unique_lock<std::mutex> lock(mutex);
	std::cout << stream.str() << std::endl;
}

} //  namespace Log
