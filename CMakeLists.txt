cmake_minimum_required(VERSION 3.5)

# set project title
project(SimMap VERSION 1.0 DESCRIPTION "A library to access map data and organize traffic simulation agents")

# set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set options
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)
option(BUILD_FOR_COVERAGE "Builds the project with coverage test options." OFF)
option(BUILD_GTEST "Sets or unsets the option to build gtest manually (do not set if gtest is installed)" OFF)
option(CREATE_DOXYGEN_TARGET "Creates the doxygen documentation if set." OFF)
option(BUILD_MAP_SERVER "Builds the map server executable." OFF)
option(BUILD_MAP_EXPORTER "Builds the map exporter executable." OFF)

# add ./cmake to CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})



# ------------------------------------------------------------------------------
# coverage
# ------------------------------------------------------------------------------

if (BUILD_FOR_COVERAGE)
    include(ModuleCoverage)
endif (BUILD_FOR_COVERAGE)


# ------------------------------------------------------------------------------
# doxygen
# ------------------------------------------------------------------------------

if (CREATE_DOXYGEN_TARGET)

    # message
    message("-- Generation of doxygen target enabled")

    # Require dot, treat the other components as optional
    find_package(Doxygen
            REQUIRED dot
            OPTIONAL_COMPONENTS mscgen dia)

    if (DOXYGEN_FOUND)

        # create doc directory
        file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)

        # settings
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_MAN YES)
        set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)

        # create target
        doxygen_add_docs(
                doxygen
                ${PROJECT_SOURCE_DIR}/src README.md
                COMMENT "Generate man pages"
        )

    endif (DOXYGEN_FOUND)

endif (CREATE_DOXYGEN_TARGET)



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
add_subdirectory(lib/Spiral)
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
if (BUILD_TESTS)
    include(ModuleGtest)
endif (BUILD_TESTS)
