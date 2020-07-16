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
target_link_libraries(AdapterTest PRIVATE odradapter)
target_include_directories(AdapterTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${odrparser_INCLUDE_DIR})

# add test
add_gtest(AdapterTest)