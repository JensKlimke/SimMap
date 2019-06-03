# source files
set(SOURCE_FILES
        LaneSectionTest.cpp
        AdapterTest.cpp
        EdgeTest.cpp
        JunctionTest.cpp
        SimplePathTest.cpp
        PathTest.cpp
        LaneSeparationTest.cpp
        )


# build test executable
add_executable(AdapterTest ${SOURCE_FILES})
target_include_directories(AdapterTest PRIVATE ${SIMMAP_INCL_DIR} ${odr_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src/odradapter)
target_link_libraries(AdapterTest PRIVATE odradapter)

# add tests
add_gtest(AdapterTest)