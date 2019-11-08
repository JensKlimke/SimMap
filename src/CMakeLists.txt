# add subdirectories
add_subdirectory(curve)
add_subdirectory(graph)
add_subdirectory(server)
add_subdirectory(odradapter)
add_subdirectory(SimMap)

include_directories(${simmap_INCLUDE_DIR})
include_directories(${Eigen_INCLUDE_DIR})