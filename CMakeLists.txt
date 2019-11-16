cmake_minimum_required(VERSION 3.5)

# set project title
project(SimMap)

# set C++ standard
set(CMAKE_CXX_STANDARD 14)

# set options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(ENABLE_COVERAGE "Enable the coverage check of the module" OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# update submodules
include(UpdateGitSubmodules)
set(GIT_SUBMODULE OFF)

# load macros for docs
set(DOC_SOURCE_FOLDERS "${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src")
include(GenerateDocs)

# create and set binary directory
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin CACHE PATH "BIN_DIR")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})


# ------------------------------------------------------------------------------
# variables
# ------------------------------------------------------------------------------

if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(WIN32)


# ------------------------------------------------------------------------------
# libraries
# ------------------------------------------------------------------------------

# cache build tests
set(MAIN_BUILD_TESTS ${BUILD_TESTS})
set(BUILD_TESTS OFF)

# load library
add_subdirectory(lib/odrparser)

set(Eigen_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/Eigen/)
set(odrparser_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/odrparser/include)

# reset build tests
set(BUILD_TESTS ${MAIN_BUILD_TESTS})



# ------------------------------------------------------------------------------
# core sources
# ------------------------------------------------------------------------------

# add sources
add_subdirectory(src)



# ------------------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------------------

# enable google testing for project
if(BUILD_TESTS)

    message("-- Testing enabled")

    # enable testing
    enable_testing()

    # unset for all subprojects
    set(BUILD_TESTS OFF)

    # add gtest sources
    add_subdirectory(lib/gtest)

    # load macros for test
    include(AddGoogleTest)

    # add test folder
    add_subdirectory(test)

endif()
