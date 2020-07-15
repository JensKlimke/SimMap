# source files
set(SOURCE_FILES
        simmap.cpp
        )

# create shared library
add_library(simmap SHARED ${SOURCE_FILES})

# include directories
target_include_directories(simmap PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

# link libraries
target_link_libraries(simmap PRIVATE odradapter)

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS simmap EXPORT SimMapConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
install(EXPORT SimMapConfig DESTINATION share/SimMap/cmake)

# This makes the project importable from the build directory
export(TARGETS simmap FILE SimMapConfig.cmake)


# definitions for windows
if (WIN32)

    # apply export or import
    add_definitions(-DMAKE_SHARED -DLIBRARY_EXPORT)

    # MT instead of MD
    if (${CMAKE_VERSION} VERSION_LESS "3.15.0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    else ()
        set_property(TARGET simmap PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif ()

endif(WIN32)
