/**
 * @file event_type.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_FILESYSTEM_EVENT_TYPE_H_
#define INC_FILESYSTEM_EVENT_TYPE_H_

#include <stdint.h>
#include <string>

#include <sys/inotify.h>


namespace Filesystem {

typedef uint32_t event_mask;

/* 
 * Helper enum for dealing with inotify's masks
 */
enum class event_type : uint32_t {
    no_event        = 0x00000000,
    access          = IN_ACCESS,
    attrib          = IN_ATTRIB,
    close_write     = IN_CLOSE_WRITE,
    close_nowrite   = IN_CLOSE_NOWRITE,
    close           = IN_CLOSE,
    create          = IN_CREATE,
    delete_         = IN_DELETE,
    delete_self     = IN_DELETE_SELF,
    modify          = IN_MODIFY,
    move_self       = IN_MOVE_SELF,
    moved_from      = IN_MOVED_FROM,
    moved_to        = IN_MOVED_TO,
    move            = IN_MOVE,
    open            = IN_OPEN,
    is_dir          = IN_ISDIR,
    unmount         = IN_UNMOUNT,
    q_overflow      = IN_Q_OVERFLOW,
    ignored         = IN_IGNORED,
    oneshot         = IN_ONESHOT,
    all_events      = IN_ALL_EVENTS
};

constexpr event_type operator&(event_type left, event_type right) {
    return static_cast<event_type>(
        static_cast<std::underlying_type<event_type>::type>(
            left) &
        static_cast<std::underlying_type<event_type>::type>(
            right));
}

constexpr bool operator&(event_mask left, event_type right) {
    return static_cast<bool>(
        left &
        static_cast<std::underlying_type<event_type>::type>(
            right));
}

constexpr event_type operator|(event_type left, event_type right) {
    return static_cast<event_type>(
        static_cast<std::underlying_type<event_type>::type>(left)
        | static_cast<std::underlying_type<event_type>::type>(right));
}

template <typename in>
constexpr event_mask to_event_mask(in arg) noexcept {
    return static_cast<std::underlying_type_t<in>>(arg);
}

std::string event_mask_to_string(event_mask mask);

}  // namespace Filesystem

#endif  // INC_FILESYSTEM_EVENT_TYPE_H_
