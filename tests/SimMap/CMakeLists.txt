set(SOURCE_FILES
        LibTest.cpp
        SpeedTest.cpp)


add_executable(LibTest ${SOURCE_FILES})
target_include_directories(LibTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(LibTest PRIVATE simmap)

add_gtest(LibTest)