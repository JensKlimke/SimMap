# set source files
set(SOURCE_FILES
        main.cpp
        )

# set proto files
set(PROTO_FILES
        simmap.proto
        )

# generate proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# create target
add_executable(mapserver
        ${SOURCE_FILES}
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        )

message(STATUS ${Protobuf_LIBRARIES})

# link libraries
target_link_libraries(mapserver PRIVATE
        ${Protobuf_LIBRARIES}
        )

# include directory
target_include_directories(mapserver PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}/src
        )