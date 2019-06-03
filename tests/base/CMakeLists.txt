# get all header files (only done to register the headers to the project)
file(GLOB HEADER_FILES
            ${SIMMAP_INCL_DIR}/base/*.h
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
target_include_directories(BaseTest PRIVATE ${SIMMAP_INCL_DIR})

# add tests
add_gtest(BaseTest)