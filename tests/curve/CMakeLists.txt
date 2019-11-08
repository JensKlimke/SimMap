
set(SOURCE_FILES
        CurveTest.cpp
        SplineTest.cpp
        )

# build test executable
add_executable(CurveTest ${SOURCE_FILES})
target_include_directories(CurveTest PRIVATE ${simmap_INCLUDE_DIR})
target_link_libraries(CurveTest PRIVATE curve)


# add tests
target_link_libraries(CurveTest PUBLIC gtest gmock gtest_main)
add_test(CurveTest CurveTest)
set_target_properties(CurveTest PROPERTIES FOLDER "Tests")