add_subdirectory(curve)
add_subdirectory(graph)
add_subdirectory(odradapter)
add_subdirectory(server)
add_subdirectory(simmap)

if (BUILD_MAP_SERVER)
    add_subdirectory(mapserver)
endif (BUILD_MAP_SERVER)

if (BUILD_MAP_EXPORTER)
    add_subdirectory(exporter)
endif (BUILD_MAP_EXPORTER)