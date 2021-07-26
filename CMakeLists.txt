
cmake_minimum_required(VERSION 3.10)
project(backupd)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

option(BACKUPD_TEST "Build tests" OFF)

# Setting version vars
add_definitions(
	-DSOFTWARE_MAJOR=1
	-DSOFTWARE_MINOR=0
	-DSOFTWARE_BUILD=0
)

# Setting compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER /usr/bin/g++-7)
set(CMAKE_C_COMPILER /usr/bin/gcc-7)
set(CXX_FILESYSTEM_LIBRARIES "stdc++fs")

find_package (Boost COMPONENTS 
    system
    program_options
REQUIRED)



# Grabbing all files
include_directories("inc")
file(GLOB_RECURSE SRC_FILES src/*.cpp src/log/*.cpp src/filesystem/*.cpp)
file(GLOB_RECURSE INC_FILES inc/*.h inc/log/*.h inc/filesystem/*.h)

# Build configuration
add_definitions(-Wall)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})
target_link_libraries(
    ${PROJECT_NAME}
    stdc++fs
    boost_program_options
    boost_system
    pthread
)

# Installation
set(CMAKE_INSTALL_PREFIX "/usr")
install(TARGETS "${PROJECT_NAME}" RUNTIME DESTINATION bin)

# Tests
if(BACKUPD_TEST)
    set(TARGET_TEST "${PROJECT_NAME}-test")
    enable_testing()

    # ----- acutest lib -----
    include_directories("test/libs/acutest/include")
    file(GLOB_RECURSE acutest_SRC ./test/libs/acutest/include/*.h)
    add_library(acutest ${acutest_SRC})
    set_target_properties(acutest PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(acutest PROPERTIES FOLDER test/libs)

    # ----- tests -----
    file(GLOB_RECURSE TESTSRC test/src/*.cpp )
    list(FILTER SRC_FILES EXCLUDE REGEX ".*backupd.cpp$")
    add_executable(${TARGET_TEST} ${TESTSRC} ${SRC_FILES} ${INC_FILES})
    target_link_libraries(
        ${TARGET_TEST}
        acutest
        stdc++fs
        boost_system
        boost_program_options
        pthread
    )
    set_target_properties(${TARGET_TEST} PROPERTIES FOLDER test)
    add_test(NAME "${PROJECT_NAME}-test" COMMAND ${TARGET_TEST})
endif()  # BACKUPD_TEST 
