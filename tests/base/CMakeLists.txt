# get all header files (only done to register the headers to the project)
file(GLOB HEADER_FILES
            ${simmap_INCLUDE_DIR}/base/*.h
        )

# source files
set(SOURCE_FILES
        FunctionsTest.cpp
        PolyTest.cpp
        SequenceTest.cpp
        NestedSequenceTest.cpp
        )

# build test executable
add_executable(BaseTest ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(BaseTest PRIVATE ${simmap_SOURCE_DIR} ${Eigen_INCLUDE_DIR})

# add tests
target_link_libraries(BaseTest PUBLIC gtest gmock gtest_main)
add_test(BaseTest BaseTest)
set_target_properties(BaseTest PROPERTIES FOLDER "Tests")