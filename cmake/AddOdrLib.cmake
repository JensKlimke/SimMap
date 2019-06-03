# 
#
# Downloads operndrive-parser and provides a helper macro to add tests. Add make check, as well, which
# gives output on failed tests without having to set an environment variable.
#
#

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(ODRLIB_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
message("Downloading odr-parser lib ...")

if (CMAKE_VERSION VERSION_LESS 3.11)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

    include(DownloadProject)
    download_project(PROJ odr
            GIT_REPOSITORY https://github.com/JensKlimke/odrparser.git
            GIT_TAG v0.1.1
            UPDATE_DISCONNECTED 1
            QUIET
            )

    # CMake warning suppression will not be needed in version 1.9
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
    add_subdirectory(${odr_SOURCE_DIR} ${odr_BINARY_DIR} EXCLUDE_FROM_ALL)
    unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)

else ()
    include(FetchContent)
    FetchContent_Declare(odr
            GIT_REPOSITORY https://github.com/JensKlimke/odrparser.git
            GIT_TAG v0.1.1)
    FetchContent_GetProperties(odr)
    if (NOT odr_POPULATED)
        FetchContent_Populate(odr)
        set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
        add_subdirectory(${odr_SOURCE_DIR} ${odr_BINARY_DIR} EXCLUDE_FROM_ALL)
        unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    endif ()
endif ()


set(odr_INCLUDE_DIR ${odr_SOURCE_DIR}/include/ CACHE DIRECTORY "Include Directory of the OpneDRIVE parser" FORCE)
include_directories(${odr_INCLUDE_DIR})


set_target_properties(odr
        PROPERTIES FOLDER "Extern")
