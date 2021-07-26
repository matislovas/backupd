/**
 * @file event_type.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <type_traits>

#include "filesystem/event_type.h"

namespace Filesystem {

std::string event_mask_to_string(event_mask mask) {
    std::string ret;

    if (mask & event_type::no_event) {
        ret += "no_event";
        return ret;
    }

    if (mask & event_type::access) {
        ret += "access ";
    }

    if (mask & event_type::attrib) {
        ret += "attrib ";
    }

    if (mask & event_type::close_write) {
        ret += "close_write ";
    }

    if (mask & event_type::close_nowrite) {
        ret += "close_nowrite ";
    }

    if (mask & event_type::close) {
        ret += "close ";
    }

    if (mask & event_type::create) {
        ret += "create ";
    }

    if (mask & event_type::delete_) {
        ret += "delete ";
    }

    if (mask & event_type::delete_self) {
        ret += "delete_self ";
    }

    if (mask & event_type::modify) {
        ret += "modify ";
    }

    if (mask & event_type::move_self) {
        ret += "move_self ";
    }

    if (mask & event_type::moved_from) {
        ret += "moved_from ";
    }

    if (mask & event_type::moved_to) {
        ret += "moved_to ";
    }

    if (mask & event_type::move) {
        ret += "move ";
    }

    if (mask & event_type::open) {
        ret += "open ";
    }

    if (mask & event_type::is_dir) {
        ret += "is_dir ";
    }

    if (mask & event_type::unmount) {
        ret += "unmount ";
    }

    if (mask & event_type::q_overflow) {
        ret += "q_overflow ";
    }

    if (mask & event_type::ignored) {
        ret += "ignored ";
    }

    if (mask & event_type::oneshot) {
        ret += "oneshot ";
    }

    // Erase last space
    ret.pop_back();

    return ret;
}

}  // namespace Filesystem
