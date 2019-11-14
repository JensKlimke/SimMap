set(SOURCE_FILES
        LibTest.cpp
        LoadLibTest.cpp
        SpeedTest.cpp
        )


# build test executable
add_executable(LibTest ${SOURCE_FILES})
target_link_libraries(LibTest PRIVATE simmap)
target_include_directories(LibTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${Eigen_INCLUDE_DIR})

# set runtime library
set_property(TARGET LibTest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# add test
add_gtest(LibTest)

