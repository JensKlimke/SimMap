# source files
set(SOURCE_FILES
            BasicPathTest.cpp
            GraphTest.cpp
            NeighbordTest.cpp
            OrientedTest.cpp
        )

# build test executable
add_executable(GraphTest ${SOURCE_FILES})
target_include_directories(GraphTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(GraphTest PRIVATE graph)

# add tests
add_gtest(GraphTest)