cmake_minimum_required(VERSION 3.10)
# based on https://cliutils.gitlab.io/modern-cmake/

project(SimMap VERSION 0.1
        DESCRIPTION "A traffic simulation map framework"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)


# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# unset testing for libs
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)

# set include dir
set(Eigen_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/Eigen)
set(odrparser_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/odrparser/include)

# add source folder
add_subdirectory(src)

### GTEST
if(BUILD_TESTS)

    message("-- Testing of SimCore enabled")

    # enable testing
    enable_testing()

    # add subdirector (gtest)
    add_subdirectory(lib/gtest)

    # load macros for tests
    include(AddGoogleTest)

    ### TESTS
    add_subdirectory(tests)

endif()

# add odrparser
SET(BUILD_TESTS OFF)
add_subdirectory(lib/odrparser)