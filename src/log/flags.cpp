/**
 * @file flags.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include "log/flags.h"

namespace Log {
namespace Flags {

const char* flag_to_string(LogFlag flag) {
    switch (flag) {
        case(EVENT) :
            return "EVENT";
        case(ERROR) :
            return "ERROR";
        case(WARNING) :
            return "WARNING";
        case(DEBUG) :
            return "DEBUG";
        default:
            return "UNKNOWN";
	}
}

}  // namespace Flags
}  // namespace Log
