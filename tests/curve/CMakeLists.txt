
set(SOURCE_FILES
        CurveTest.cpp
        SplineTest.cpp
        )

# build test executable
add_executable(CurveTest ${SOURCE_FILES})
target_link_libraries(CurveTest PRIVATE curve)
target_include_directories(CurveTest PRIVATE ${PROJECT_SOURCE_DIR}/src ${Eigen_INCLUDE_DIR})

# add test
add_gtest(CurveTest)