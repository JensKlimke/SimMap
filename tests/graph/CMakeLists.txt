# source files
set(SOURCE_FILES
            BasicPathTest.cpp
            NeighbordTest.cpp
            OrientedTest.cpp
        )

# build test executable
add_executable(GraphTest ${SOURCE_FILES})
target_link_libraries(GraphTest PRIVATE graph)
target_include_directories(GraphTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${Eigen_INCLUDE_DIR})

# add test
add_gtest(GraphTest)