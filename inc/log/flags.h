/**
 * @file flags.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_LOG_FLAGS_H_
#define INC_LOG_FLAGS_H_

#include <stdint.h>

namespace Log {
namespace Flags {

typedef uint32_t LogFlag;

const LogFlag EVENT	= 1 << 0;
const LogFlag ERROR	= 1 << 1;
const LogFlag WARNING  = 1 << 2;
const LogFlag DEBUG	= 1 << 3;

const char* flag_to_string(LogFlag flag);

}  // namespace Flags
}  // namespace Log

#endif  // INC_LOG_FLAGS_H_
