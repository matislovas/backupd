# backupd
Auto directory backup daemon for Linux - based systems.
Makes a backup copy of any file created, modified, moved-to the `--dir` directory, to `--bak` directory (with `.bak` extension).

Allowed args:
```sh
-h [ --help ]         show help
-v [ --version ]      show version
-D [ --debug ]        enable debug logging
-s [ --silent ]       disable console log
-d [ --dir ] arg      directory to monitor
-b [ --bak ] arg      directory to create backups to
-l [ --log ] arg      log file path
```

Note:

1. Files that are inside of monitored, prior to application launch, won't be backed-up.
2. Directories, hidden files (with filename `.name.ext`), irregular files (links, devices) are ignored.
3. Files prefixed with `delete_` will be deleted immediately, their corresponding file (w/o prefix) will be deleted too, as well as the backed-up file.
4. Files prefixed with `delete_ISODATETIME_` (e.g. `delete_2021-07-30T12:13:14_`) will be deleted at specified time (if, of course, that time is in future).

## Dependencies

Install the required dependencies (instructions tested on Ubuntu 18.04).

Update sources:
```sh
$ apt update
```

Boost libs:
```sh
$ apt install libboost-all-dev
```

Build tools:

```sh
$ apt install build-essential cmake
```

## Build and install

Clone the repo to directory of your choice:
```sh
$ git clone https://github.com/matislovas/backupd.git && cd backupd
```

Prepare build env:
```sh
$ mkdir build && cd build
```

Run cmake (with some additional flags, if you like):
```sh
$ cmake [-DCMAKE_BUILD_TYPE=Debug] ..
```

Than build:
```sh
$ make
```

If you wish, you can install it in `/usr/bin`:
```sh
$ make install
```

## Tests

Init `acutest` submodule:
```sh
$ git submodule update --init
```

Run cmake with tests enabled:
```sh
$ cd build && cmake [-DBACKUPD_TEST=ON] ..
```

Run actual tests:

```sh
$ ../bin/backupd-test
```
