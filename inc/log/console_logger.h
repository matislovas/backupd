/**
 * @file console_logger.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_CONSOLE_LOGGER_H_
#define INC_LOG_CONSOLE_LOGGER_H_

#include <memory>
#include <mutex>

#include "log/ilog_output.h"

namespace Log {

/**
* ilog_output that prints all log messages to the console
*/
class console_logger final : public ilog_output {
  public:
    console_logger() {}

	static std::shared_ptr<ilog_output> create() {
		return std::make_shared<console_logger>();
	};

	void log(const entry& block) override;

  private:
	std::mutex mutex;
};

} //  namespace Log

#endif  // INC_LOG_CONSOLE_LOGGER_H_
