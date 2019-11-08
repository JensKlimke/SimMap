# get all header files
file(GLOB HEADER_FILES
        ${simmap_INCLUDE_DIR}/SimMap/*.h
        )

add_library(simmap STATIC simmap_lib.cpp ${HEADER_FILES})
target_include_directories(simmap PRIVATE ${simmap_INCLUDE_DIR} ${simmap_INCLUDE_DIR}/SimMap)
target_link_libraries(simmap PRIVATE server odradapter)