# get all header files
file(GLOB HEADER_FILES
        ${simmap_INCLUDE_DIR}/graph/*.h
        )

set(SOURCE_FILES
        Neighbored.cpp
        Oriented.cpp
        Edge.cpp
        Path.cpp
        Graph.cpp
        )

add_library(graph STATIC ${SOURCE_FILES})
target_include_directories(graph PRIVATE ${simmap_INCLUDE_DIR} ${simmap_INCLUDE_DIR}/graph)