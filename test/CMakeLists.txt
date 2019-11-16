# add tracks directory
add_definitions(-DTRACKS_DIR=\"${PROJECT_SOURCE_DIR}/test/tracks\")

add_subdirectory(base)
add_subdirectory(curve)
add_subdirectory(graph)
add_subdirectory(odrapater)
add_subdirectory(simmap)