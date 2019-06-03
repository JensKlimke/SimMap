# get all header files
file(GLOB HEADER_FILES
        ${SIMMAP_INCL_DIR}/server/*.h
        )

set(SOURCE_FILES
        MapCoordinate.cpp
        LaneEdge.cpp
        Path.cpp
        Track.cpp
        )

add_library(server STATIC ${SOURCE_FILES})
target_link_libraries(server PRIVATE graph)
target_include_directories(server PRIVATE ${SIMMAP_INCL_DIR} ${SIMMAP_INCL_DIR}/server)