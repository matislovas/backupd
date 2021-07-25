/**
 * @file file_logger.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_FILE_LOGGER_H_
#define INC_LOG_FILE_LOGGER_H_

#include <memory>
#include <mutex>
#include <fstream>
#include <experimental/filesystem>

#include "log/ilog_output.h"

namespace Log {

namespace fs = std::experimental::filesystem;

/**
* ilog_output that prints all log messages to a file
*/
class file_logger final : public ilog_output {
  public:
    file_logger(const fs::path& path);
    file_logger() = delete;

    ~file_logger();

	static std::shared_ptr<ilog_output> create(fs::path path) {
		return std::make_shared<file_logger>(path);
	};

	void log(const entry& block) override;


  private:
    std::fstream file;
    std::mutex mutex;
};

} //  namespace Log

#endif  // INC_LOG_FILE_LOGGER_H_
