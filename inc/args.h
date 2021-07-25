/**
 * @file args.h
 * @author Matas Misiunas
 * @copyright Copyright 2021 by Matas. All rights reserved.
 * @date Jul 25, 2021
 */


#ifndef INC_ARGS_H_
#define INC_ARGS_H_

#include <string>

#include <boost/program_options.hpp>


class args {
  public:
    /**
     * Parse the command line parameters and configuration file contents
     * @param argc
     * @param argv
     */
    static void create_handle(int argc, char *argv[]);

    /**
     * Prints the software version to standard output
     */
    static void version();

    /**
     * Checks whether a command line parameter is set
     * @param flag  Parameter name
     * @return      True if set
     */
    static bool flag_enabled(std::string flag);

    /**
     * Returns command line or configuration variable
     * @param name  Variable name
     * @return      Variable value as type <T>
     */
    template<typename T> static T get(std::string name) {
        if (!flag_enabled(name)) {
        throw std::runtime_error(
            std::string("Could not find configuration variable: ") + name);
        }

        try {
            return var_map_[name.c_str()].as<T>();
        } catch (std::exception &e) {
            throw std::runtime_error(
                std::string("Failed to get variable: ") + name);
        }
    }


  private:
    static boost::program_options::variables_map var_map_;
};

#endif  // INC_ARGS_H_
