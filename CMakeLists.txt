cmake_minimum_required(VERSION 3.5)

# set project title
project(SimMap VERSION 1.0 DESCRIPTION "A library to access map data and organize traffic simulation agents")

# set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(BUILD_FOR_COVERAGE "Builds the project with coverage test options." OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# ------------------------------------------------------------------------------
# documentation
# ------------------------------------------------------------------------------

# load macros for docs
set(DOC_SOURCE_FOLDERS "${PROJECT_SOURCE_DIR}/include")
include(GenerateDocs)


# ------------------------------------------------------------------------------
# coverage
# ------------------------------------------------------------------------------

if (BUILD_FOR_COVERAGE)

    # message
    message("-- Coverage option is enabled")

    # gnu or clang
    if (CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "--coverage")
    elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang"
            OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    endif ()

    # from: https://blog.jetbrains.com/clion/2019/10/clion-2019-3-eap-coverage-cmake-defaults-lldb/

endif (BUILD_FOR_COVERAGE)



# ------------------------------------------------------------------------------
# shared library for windows (todo)
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
