# get all header files
file(GLOB HEADER_FILES
        ${SIMMAP_INCL_DIR}/SimMap/*.h
        )

add_library(simmap STATIC simmap_lib.cpp ${HEADER_FILES})
target_include_directories(simmap PRIVATE ${SIMMAP_INCL_DIR} ${SIMMAP_INCL_DIR}/SimMap)
target_link_libraries(simmap PRIVATE server odradapter)