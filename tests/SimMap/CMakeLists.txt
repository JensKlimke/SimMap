add_executable(LibTest LibTest.cpp)
target_include_directories(LibTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(LibTest PRIVATE simmap)

add_gtest(LibTest)

# Note: add_gtest is just a macro that adds gtest, gmock, and gtest_main, and then runs add_test to create a test with the same name:
# target_link_libraries(SimpleTest gtest gmock gtest_main)
# add_test(SimpleTest SimpleTest)