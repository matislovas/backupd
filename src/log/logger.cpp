/**
 * @file logger.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <algorithm>

#include "log/logger.h"

namespace Log {

logger::logger(filter filters_):
    filters(filters_) {}

void logger::set_filters(filter filters_) {
    this->filters = filters_;
}

void logger::add_output(std::shared_ptr<ilog_output> output) {
    // Avoid double inlcusion
    if (std::find(
            this->outputs.begin(),
            this->outputs.end(),
        output) == this->outputs.end()) {
        this->outputs.push_back(output);
    }
}

void logger::output(
    Flags::LogFlag flag,
    const char* prefix,
    const char* msg) {
    for (auto & output : this->outputs) {
        output->log(
            entry(flag, prefix, msg));
    }
}

bool logger::is_enabled(Flags::LogFlag filter_flag) {
    if (this->outputs.empty()) {
        return false;
    }

    if (!this->filters.is_set(filter_flag)) {
        return false;
    }

    return true;
}

}  // namespace Log
