
# source files
set(SOURCE_FILES
        FunctionsTest.cpp
        PolyTest.cpp
        SequenceTest.cpp
        NestedSequenceTest.cpp
        )

# build test executable
add_executable(BaseTest ${SOURCE_FILES})
include_directories(BaseTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${Eigen_INCLUDE_DIR})

# add test
add_gtest(BaseTest)