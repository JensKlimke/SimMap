# source files
set(SOURCE_FILES
            BasicPathTest.cpp
            NeighbordTest.cpp
            OrientedTest.cpp
        )

# build test executable
add_executable(GraphTest ${SOURCE_FILES})
target_include_directories(GraphTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(GraphTest PRIVATE graph)

# add tests
target_link_libraries(GraphTest PUBLIC gtest gmock gtest_main)
add_test(GraphTest GraphTest)
set_target_properties(GraphTest PROPERTIES FOLDER "Tests")