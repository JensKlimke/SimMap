set(SOURCE_FILES
        Neighbored.cpp
        Oriented.cpp
        Edge.cpp
        Path.cpp
        Graph.cpp
        )

add_library(graph STATIC ${SOURCE_FILES})
target_include_directories(graph PRIVATE ${PROJECT_SOURCE_DIR}/src)