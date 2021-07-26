/**
 * @file backup_handler.cpp
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */

#include <iomanip>
#include <regex>
#include <ctime>

#include "filesystem/backup_handler.h"


#define LOG_PREFIX "backup_handler"

namespace Filesystem {

backup_handler::backup_handler(
    boost::asio::io_service& io_,
    Log::logger& logger_,
    fs::path mondir_path_,
    fs::path workdir_path_):
    mondir_path(mondir_path_),
    workdir_path(workdir_path_),
    io(io_),
    logger(logger_) {
    // TODO do we need to copy all files from mondir to workdir at init?
}

void backup_handler::event_interface(
    const boost::system::error_code &err,
    event ev) {
    if (err) {
        return;
    }

    // TODO no action on directories ..?
    if (ev.type & event_type::is_dir) {
        return;
    }

    // No action on irregular files ...
    if (!fs::is_regular_file(ev.filepath) && ev.filepath.has_filename()) {
        return;
    }

    // TODO ignore hidden files ..?
    if (ev.filepath.filename().string().at(0) == '.') {
        return;
    }

    LOG(
        this->logger,
        Log::Flags::EVENT,
        "Got event, type: %s, path: %s",
        event_mask_to_string(ev.type).c_str(),
        ev.filepath.c_str());

    std_clock::time_point del_tp;

    auto action = this->check_for_delete(ev.filepath.filename(), &del_tp);

    // 1. Check for delete action:
    //  1.1 Check for "delete_" prefix
    //  1.2 Check for "ISODATETIME_" prefix
    //  Result: delete file in mon_dir (w and w/o prefix), work_dir (w/o prefix) and return.
    // 2. If not delete state, copy and overwirte file to work_dir and return

    // TODO we're getting multiple events for ::modify, solve it!
    if ((ev.type & event_type::modify) ||
        (ev.type & event_type::moved_to) ||
        (ev.type & event_type::create)) {
        if (action == delete_action::delete_now) {
            LOG(
                this->logger,
                Log::Flags::DEBUG,
                "Deleting file now, path: %s",
                ev.filepath.c_str());

            this->delete_backup(
                ev.filepath.filename(),
                this->strip_prefix(
                    ev.filepath.filename()));
        } else if (action == delete_action::delete_later) {
            std::time_t future = sys_clock::to_time_t(
                sys_clock::now() +
                (del_tp - std_clock::now()));

            std::stringstream ss;
            ss << std::put_time(std::localtime(&future), "%Y-%m-%d %H:%M:%S");

            LOG(
                this->logger,
                Log::Flags::DEBUG,
                "Deleting file later at %s, path: %s",
                ss.str().c_str(),
                ev.filepath.c_str());
            
            dt timer = std::make_shared<waitable_timer>(this->io, del_tp);

            // Hash, for timer uid in map
            size_t filename_h = std::hash<std::string>{}(ev.filepath.filename());

            auto callback = [this,filename_h,ev]
                (const boost::system::error_code& err) -> void {
                LOG(
                    this->logger,
                    Log::Flags::DEBUG,
                    "Delayed deletion, path: %s",
                    ev.filepath.c_str());

                this->delete_backup(
                    ev.filepath.filename(),
                    this->strip_prefix(
                        ev.filepath.filename()));

                // Remove delayed job from map
                this->timers.erase(filename_h);
            };

            timer->async_wait(callback);

            this->timers.insert({filename_h, timer});
        } else {
            LOG(
                this->logger,
                Log::Flags::DEBUG,
                "Backup file, path: %s",
                ev.filepath.c_str());

            backup_file(ev.filepath.filename());
        }
    }
}

delete_action backup_handler::check_for_delete(
    std::string filename,
    std_clock::time_point* time) {
    delete_action ret = delete_action::no_action;
    std::string search_delete(BACKUPD_FILE_DEL_NAME);

    auto found_delete = filename.find(search_delete);

    if (found_delete != std::string::npos && found_delete == 0) {
        ret = delete_action::delete_now;

        // Searching for ISODATETIME, starting after "delete_", searching for another "_"
        auto found_time = filename.find("_", found_delete + search_delete.size());

        if (found_time != std::string::npos && found_time > search_delete.size()) {
            // Found another underscore, extracting substring and checking if it is a valid datetime value
            std::tm tm;

            std::stringstream datetime_ss(
                filename.substr(
                    search_delete.size(),
                    found_time - search_delete.size()));

            // Validating date format
            if (std::regex_match(datetime_ss.str(), this->r_datetime)) {
                // ISODATETIME format "2019-09-27T04:00:00"
                datetime_ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

                auto future = sys_clock::from_time_t(
                    std::mktime(
                        &tm));

                if (future < sys_clock::now()) {
                    // Delete time is in the past, so procede with immediate deletion
                    return ret;
                }

                ret = delete_action::delete_later;

                // System clock to duration, then getting time point in the future.
                *time = 
                    std_clock::now() + 
                    std::chrono::duration_cast<std_clock::duration>(
                        future - sys_clock::now());
            }
        }
    }

    return ret;
}

bool backup_handler::backup_file(std::string filename) {
    bool result = false;
    auto src = this->mondir_path;
    auto dst = this->workdir_path;

    src += filename;
    dst += filename;
    dst += BACKUPD_FILE_COPY_EXT;

    try {
        result = fs::copy_file(
            src,
            dst,
            fs::copy_options::overwrite_existing);
    } catch (fs::filesystem_error const& err) {
        LOG(
            this->logger,
            Log::Flags::WARNING,
            "File backup failed: %s",
            err.what());
    }

    return result;
}

void backup_handler::delete_backup(
    std::string filename_w_pref,
    std::string filename_orig) {
    std::vector<fs::path> to_delete;
    // Remove prefixed file, unprefixed file and backuped file
    fs::path src_w_pref = this->mondir_path;
    src_w_pref += filename_w_pref;

    to_delete.push_back(src_w_pref);
    
    fs::path src_orig;
    fs::path bckp_orig;

    if (!filename_orig.empty()) {
        src_orig = this->mondir_path;
        src_orig += filename_orig;

        bckp_orig = this->workdir_path;
        bckp_orig += filename_orig;
        bckp_orig += BACKUPD_FILE_COPY_EXT;

        to_delete.push_back(src_orig);
        to_delete.push_back(bckp_orig);
    }

    for (auto & del : to_delete) {
        try {
            fs::remove(del);
        } catch (fs::filesystem_error const& err) {
            LOG(
                this->logger,
                Log::Flags::WARNING,
                "Delete file failed: %s, filepath: %s", 
                err.what(),
                del.c_str());
        }
    }
}

fs::path backup_handler::strip_prefix(fs::path input) {
    std::string filename = input.filename();
    std::string search_delete(BACKUPD_FILE_DEL_NAME);
    std::string datetime_substr;

    auto found_delete = filename.find(search_delete);

    // Erase "delete_" part
    if (found_delete != std::string::npos && found_delete == 0) {
        filename.erase(0, search_delete.size());
    }

    auto found_time = filename.find("_");

    // Erase "ISODATETIME_" part
    if (found_time != std::string::npos) {
        datetime_substr = filename.substr(0, found_time);

        // Validatingy ISODATETIME format
        if (std::regex_match(datetime_substr, this->r_datetime)) {
            // Delete ISODATETIME + '_'
            filename.erase(0, found_time + 1);
        }
    }

    // Return stripped path
    input = input.remove_filename();
    input += filename;

    return input;
}

}  // namespace Filesystem
