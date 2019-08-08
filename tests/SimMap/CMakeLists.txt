set(SOURCE_FILES
        LibTest.cpp
        LoadLibTest.cpp
        SpeedTest.cpp )


add_executable(LibTest ${SOURCE_FILES})
target_include_directories(LibTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(LibTest PRIVATE simmap)

# add test
target_link_libraries(LibTest PUBLIC gtest gmock gtest_main)
add_test(LibTest LibTest)
set_target_properties(LibTest PROPERTIES FOLDER "Tests")

