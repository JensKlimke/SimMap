cmake_minimum_required(VERSION 3.10)
# based on https://cliutils.gitlab.io/modern-cmake/

project(SimMap VERSION 0.1
        DESCRIPTION "Example project by Jens Klimke"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()


# first check
if(NOT DEFINED SIMMAP_ENABLE_TESTING)
    set(SIMMAP_ENABLE_TESTING true)
endif(NOT DEFINED SIMMAP_ENABLE_TESTING)


# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# get eigen (for all targets)
include(AddEigen3)

# set include dir
set(SIMMAP_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)


### GTEST
if(SIMMAP_ENABLE_TESTING)

    ### GTEST
    enable_testing()

    ### TESTS
    add_subdirectory(tests)

endif()
