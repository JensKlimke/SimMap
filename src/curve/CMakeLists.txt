# get all header files
file(GLOB HEADER_FILES
        ${simmap_INCLUDE_DIR}/curve/*.h
        )

set(SOURCE_FILES
        C3Spline.cpp
        Curve.cpp
        Spiral.cpp
        odrSpiral.cpp
        GeoElement.cpp
        )

add_library(curve STATIC ${SOURCE_FILES})
target_include_directories(curve PRIVATE ${simmap_INCLUDE_DIR})