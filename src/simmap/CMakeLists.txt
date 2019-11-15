
add_library(simmap STATIC simmap_lib.cpp)

target_include_directories(simmap PRIVATE
        ${PROJECT_SOURCE_DIR}/include/simmap
        ${PROJECT_SOURCE_DIR}/src
        ${Eigen_INCLUDE_DIR}
        ${odrparser_INCLUDE_DIR}
        )

target_link_libraries(simmap PRIVATE server odradapter)