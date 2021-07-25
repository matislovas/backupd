/**
 * @file ilog_output.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_ILOG_OUTPUT_H_
#define INC_LOG_ILOG_OUTPUT_H_

#include "log/entry.h"

namespace Log {

class ilog_output {
  public:
	virtual ~ilog_output() {}

	virtual void log(const entry& block) = 0;
};

}  // namespace Log

#endif  // INC_LOG_ILOG_OUTPUT_H_
