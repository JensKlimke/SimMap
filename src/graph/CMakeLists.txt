# get all header files
file(GLOB HEADER_FILES
        ${SIMMAP_INCL_DIR}/graph/*.h
        )

set(SOURCE_FILES
        Neighbored.cpp
        Oriented.cpp
        Edge.cpp
        Path.cpp
        Graph.cpp
        )

add_library(graph STATIC ${SOURCE_FILES})
target_include_directories(graph PRIVATE ${SIMMAP_INCL_DIR} ${SIMMAP_INCL_DIR}/graph)