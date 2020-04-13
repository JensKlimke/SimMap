set(SOURCE_FILES
        MapCoordinate.cpp
        LaneEdge.cpp
        Path.cpp
        Track.cpp
        )

add_library(server STATIC ${SOURCE_FILES})
target_link_libraries(server PRIVATE graph)
target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/src)