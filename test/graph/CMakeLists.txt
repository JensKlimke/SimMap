# source files
set(SOURCE_FILES
        BasicPathTest.cpp
        NeighbordTest.cpp
        OrientedTest.cpp
        )

# create target
add_executable(GraphTest ${SOURCE_FILES})

# include directory
target_include_directories(GraphTest PRIVATE ${PROJECT_SOURCE_DIR}/src)

# link library to target
target_link_libraries(GraphTest PRIVATE graph)

# add test
add_gtest(GraphTest)

# Note: add_gtest is just a macro that adds gtest, gmock, and gtest_main, and then runs add_test to create a test with the same name:
# target_link_libraries(SimpleTest gtest gmock gtest_main)
# add_test(SimpleTest SimpleTest)