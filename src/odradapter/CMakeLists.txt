
# get all header files
file(GLOB HEADER_FILES
        ${SIMMAP_INCL_DIR}/odradapter/*.h
        )

set(SOURCE_FILES
        LaneSectionSequence.cpp
        ODRAdapter.cpp
        ODREdge.cpp
        _parseLane.cpp
        _parseCurve.cpp
        _parseLaneOffset.cpp
        _parseLinks.cpp
        _parseLaneSections.cpp
        _parseJunction.cpp
        _parseSignals.cpp
        _parseRoadMarking.cpp)

add_library(odradapter STATIC ${SOURCE_FILES})
target_include_directories(odradapter PRIVATE ${SIMMAP_INCL_DIR} ${SIMMAP_INCL_DIR}/odradapter)
target_link_libraries(odradapter PRIVATE odr server curve)
