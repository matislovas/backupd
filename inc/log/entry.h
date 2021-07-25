/**
 * @file entry.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_ENTRY_H_
#define INC_LOG_ENTRY_H_

#include "log/filter.h"
#include "log/flags.h"

namespace Log {

struct entry {
  public:
	entry(
        const Flags::LogFlag filter_flag_,
        const char* prefix_,
        const char* message_):
		filter_flag(filter_flag_),
		prefix(prefix_),
		message(message_) {}

    entry() = delete;


	Flags::LogFlag filter_flag;
	const char* prefix;
	const char*	message;
};

}  // namespace Log

#endif  // INC_LOG_ENTRY_H_
