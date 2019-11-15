# set source files
set(SOURCE_FILES
        CurveTest.cpp
        SplineTest.cpp
        )

# create target
add_executable(CurveTest ${SOURCE_FILES})

# include directory
target_include_directories(CurveTest PRIVATE ${Eigen_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)

# link library to target
target_link_libraries(CurveTest PRIVATE curve)

# add test
add_gtest(CurveTest)

# Note: add_gtest is just a macro that adds gtest, gmock, and gtest_main, and then runs add_test to create a test with the same name:
# target_link_libraries(SimpleTest gtest gmock gtest_main)
# add_test(SimpleTest SimpleTest)