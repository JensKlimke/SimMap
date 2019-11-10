# add tracks directory
set(TRACKS_DIR ${PROJECT_SOURCE_DIR}/tests/tracks)
add_definitions(-DTRACKS_DIR=\"${TRACKS_DIR}\")

# add subdirectories
add_subdirectory(base)
add_subdirectory(curve)
add_subdirectory(graph)
add_subdirectory(odrapater)
add_subdirectory(simmap)
