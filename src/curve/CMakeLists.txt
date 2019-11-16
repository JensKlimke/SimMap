set(SOURCE_FILES
        C3Spline.cpp
        Curve.cpp
        Spiral.cpp
        odrSpiral.cpp
        GeoElement.cpp
        )

# create target
add_library(curve STATIC ${SOURCE_FILES})

# include directory
target_include_directories(curve PRIVATE ${Eigen_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)