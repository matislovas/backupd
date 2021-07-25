/**
 * @file test_logger.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iomanip>
#include <experimental/filesystem>

#include "acutest.h"

#include "log/log.h"
 
 
#define TEST_LOG_FILEPATH "./test.log"
#define LOG_PREFIX "test"

namespace fs = std::experimental::filesystem;

 
void test_log_to_file(void) {
    // Create logger obj
    Log::filter filters;
    filters.set(Log::Flags::ERROR);
    Log::logger logger(filters);
 
    logger.add_output(
        Log::file_logger::create(
            fs::absolute(
                fs::path(TEST_LOG_FILEPATH))));

    // Creating correct string
    std::stringstream correct_line;
 
    auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());

	correct_line << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << ": ";
    correct_line << "[" << Log::Flags::flag_to_string(Log::Flags::ERROR) << "] ";
    correct_line << "[" << LOG_PREFIX << "] ";
	correct_line << "   " << "test 1";
    
    // Emitting log to file
    LOG(
        logger,
        Log::Flags::ERROR,
        "test %d",
        1);
    
    // Reading from file
    std::string line;
    std::ifstream file;
    file.open(TEST_LOG_FILEPATH);
    getline(file, line);
    std::remove(TEST_LOG_FILEPATH);

    TEST_CHECK(line == correct_line.str());
}

void test_log_to_console(void) {

    // Redirect std::cout buffer to other temp stringstream
    std::stringstream tmp_cout;
    std::streambuf *sbuf = std::cout.rdbuf();
    std::cout.rdbuf(tmp_cout.rdbuf());

    // Create logger obj
    Log::filter filters;
    filters.set(Log::Flags::ERROR);
    Log::logger logger(filters);

    logger.add_output(Log::console_logger::create());

    // Creating correct string
    std::stringstream correct_line;
    auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());

	correct_line << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << ": ";
    correct_line << "[" << Log::Flags::flag_to_string(Log::Flags::ERROR) << "] ";
    correct_line << "[" << LOG_PREFIX << "] ";
	correct_line << "   " << "test 1";
    correct_line << std::endl;

    // Emitting log to console
    LOG(
        logger,
        Log::Flags::ERROR,
        "test %d",
        1);

    // Redirect cout to its old self
    std::cout.rdbuf(sbuf);

    TEST_CHECK(tmp_cout.str() == correct_line.str());
}

TEST_LIST = {
    { "test_log_to_file",                               test_log_to_file },
    { "test_log_to_console",                            test_log_to_console },
    { NULL, NULL }
};
