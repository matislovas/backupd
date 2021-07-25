/**
 * @file logger.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_LOGGER_H_
#define INC_LOG_LOGGER_H_

#include <memory>
#include <vector>
#include <stdint.h>

#include "log/ilog_output.h"
#include "log/filter.h"
#include "log/flags.h"

#ifndef LOG_ENTRY_SIZE_MAX
#define LOG_ENTRY_SIZE_MAX 256
#endif  // LOG_ENTRY_SIZE_MAX

// TODO define another macro, which takes a ptr
#define LOG(logger, filter_flag, format, ...) \
	if (logger.is_enabled(filter_flag)) { \
		char msg_buff[LOG_ENTRY_SIZE_MAX]; \
		snprintf(msg_buff, LOG_ENTRY_SIZE_MAX, format, ##__VA_ARGS__); \
		logger.output(filter_flag, LOG_PREFIX, msg_buff); \
	}


namespace Log {

class logger {
  public:
	logger(filter filters = filter());
	~logger() {}

	/**
     * Set filters mask, for which log statements to let through
     *
     * @param filters_ 	Filters mask
     *
     */
	void set_filters(filter filters_);

	/**
     * Add another output class to pool
     *
     * @param output 	Output class instance
     *
     */
	void add_output(std::shared_ptr<ilog_output> output);

	/**
     * Emit log message to outputs
     *
     * @param flag 		Message's filter flag
	 * @param prefix 	Log's prefix
	 * @param msg 		Log's message
     *
     */
	void output(
		Flags::LogFlag flag,
		const char* prefix,
		const char* msg);

	/**
     * Checks if particular filter flag is enabled in mask
     *
     * @param filter_flag 	Filter flag to check
	 * @return 				True, if flag is enabled 
     *
     */
	bool is_enabled(Flags::LogFlag filter_flag);

  private:
	filter filters;

	std::vector<std::shared_ptr<ilog_output>> outputs;
};

}  // namespace Log

#endif  // INC_LOG_LOGGER_H_
