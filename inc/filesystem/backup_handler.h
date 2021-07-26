/**
 * @file backup_handler.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#ifndef INC_FILESYSTEM_FILE_HANDLER_H_
#define INC_FILESYSTEM_FILE_HANDLER_H_

#include <memory>
#include <regex>
#include <map>

#include <boost/asio.hpp>

#include "filesystem/event.h"
#include "log/logger.h"


#define BACKUPD_FILE_COPY_EXT   ".bak"
#define BACKUPD_FILE_DEL_NAME   "delete_"


namespace Filesystem {

typedef boost::asio::basic_waitable_timer<std_clock> waitable_timer;
typedef std::shared_ptr<waitable_timer> dt;

enum class delete_action : uint8_t {
    delete_now      = 0x01,
    delete_later    = 0x02,
    no_action       = 0xFF
};

/* 
 * Class for handling events from monitored directory 
 * and doing backup logic
 */
class backup_handler {
  public:
    backup_handler(
        boost::asio::io_service& io,
        Log::logger& logger,
        fs::path mondir_path,
        fs::path workdir_path);

    backup_handler() = delete;

    /**
     * Callback for events from directory monitor
     *
     * @param err   Error code placeholder
     * @param ev    Filesystem event
     *
     */
    void event_interface(
        const boost::system::error_code &err, event ev);


  private:
    /**
     * Checks filename for delete metadata
     *
     * @param filename  Filename string (w all prefixes)
     * @param time      Time point pointer (for delayed deletion)
     * @return          Enum val for required action
     *
     */
    delete_action check_for_delete(
        std::string filename,
        std_clock::time_point* time);

    /**
     * Copies original file to backup directory
     *
     * @param filename  Original files's name string
     * @return          True, if backup succeeded
     *
     */
    bool backup_file(std::string filename);

    /**
     * Strips leading "delete_" and "ISODATETIME_" prefixes from filename
     * leaving it with only original name
     *
     * @param input  Original files's path with filename
     * @return       Path with stripped filename
     *
     */
    fs::path strip_prefix(fs::path input);

    /**
     * Deletes files in original and backup directories
     *
     * @param filename_w_pref   Original filename
     * @param filename_orig     Stripped filename
     *
     */
    void delete_backup(
        std::string filename_w_pref,
        std::string filename_orig = "");

    
    fs::path mondir_path;
    fs::path workdir_path;

    // Hash + timer
    std::map<size_t, dt> timers;

    // Not the very strict one, but should be enough for starters ...
    const std::regex r_datetime = 
        std::regex("^(\\d{4})-0?(\\d+)-0?(\\d+)[T ]0?(\\d+):0?(\\d+):0?(\\d+)$");

    boost::asio::io_service& io;
    Log::logger& logger;
};

}  // namespace Filesystem

#endif  // INC_FILESYSTEM_FILE_HANDLER_H_
