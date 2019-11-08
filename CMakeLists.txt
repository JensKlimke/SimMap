cmake_minimum_required(VERSION 3.10)
# based on https://cliutils.gitlab.io/modern-cmake/

project(SimMap VERSION 0.1
        DESCRIPTION "A traffic simulation map framework"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)


# unset testing for libs
option(BUILD_TESTS "Sets or unsets the option to generate the test target" OFF)

# set include dir
set(simmap_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(simmap_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(Eigen_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/Eigen)
set(odrparser_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib/odrparser)

# add odrparser
add_subdirectory(lib/odrparser)

# add source folder
add_subdirectory(src)


### GTEST
if(BUILD_TESTS)

    # add gtest
    add_subdirectory(lib/gtest)

    ### GTEST
    enable_testing()

    ### TESTS
    add_subdirectory(tests)

endif()
