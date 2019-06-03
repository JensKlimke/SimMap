
set(SOURCE_FILES
        CurveTest.cpp
        SplineTest.cpp
        )

# build test executable
add_executable(CurveTest ${SOURCE_FILES})
target_include_directories(CurveTest PRIVATE ${SIMMAP_INCL_DIR})
target_link_libraries(CurveTest PRIVATE curve)


# add tests
add_gtest(CurveTest)