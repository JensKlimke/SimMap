
add_library(simmap SHARED simmap_lib.cpp)

target_include_directories(simmap PRIVATE
        ${PROJECT_SOURCE_DIR}/include/simmap
        ${PROJECT_SOURCE_DIR}/src
        ${odrparser_INCLUDE_DIR}
        )

if(WIN32)

    # apply export or import
    add_definitions(-DMAKE_SHARED -DLIBRARY_EXPORT)

#    # MT instead of MD
#    if(${CMAKE_VERSION} VERSION_LESS "3.15.0")
#        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#    else()
#        set_property(TARGET simmap PROPERTY
#            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#    endif()

endif(WIN32)

target_link_libraries(simmap PRIVATE odradapter)