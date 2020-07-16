set(SOURCE_FILES
        C3Spline.cpp
        Curve.cpp
        Spiral.cpp
        GeoElement.cpp
        )

# create target
add_library(curve STATIC
        ${SOURCE_FILES}
        )

# include directory
target_include_directories(curve PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/lib/Spiral/include
        )

# add odr spiral library
target_link_libraries(curve PRIVATE
        odr_spiral
        )