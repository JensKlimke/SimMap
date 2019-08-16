cmake_minimum_required(VERSION 3.10)
# based on https://cliutils.gitlab.io/modern-cmake/

project(SimMap VERSION 0.1
        DESCRIPTION "Example project by Jens Klimke"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)


# unset testing for libs
set(ODRLIB_ENABLE_TESTING OFF CACHE BOOL "Unset testing for odrparser")

# add libraries
include_directories(lib/Eigen)
add_subdirectory(lib/odrparser)
include_directories(lib/odrparser/include/)

# set include dir
set(SIMMAP_INCL_DIR ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)


# first check if tests is enabled
if(NOT DEFINED SIMMAP_ENABLE_TESTING)
    set(SIMMAP_ENABLE_TESTING true)
endif(NOT DEFINED SIMMAP_ENABLE_TESTING)

### GTEST
if(SIMMAP_ENABLE_TESTING)

    # add gtest
    add_subdirectory(lib/gtest)

    ### GTEST
    enable_testing()

    ### TESTS
    add_subdirectory(tests)

endif()
