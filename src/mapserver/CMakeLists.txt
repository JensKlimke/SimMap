# activate modules
include(ModuleProtobuf)
include(ModuleGrpc)

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
PROTOBUF_GENERATE_GRPC_CPP(PROTO_GRPC_SRCS PROTO_GRPC_HDRS ${PROTO_FILES})

# create target
add_executable(mapserver
        ${SOURCE_FILES}
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${PROTO_GRPC_SRCS}
        ${PROTO_GRPC_HDRS}
        )

# link libraries
target_link_libraries(mapserver PRIVATE
        ${Protobuf_LIBRARIES}
        ${gRPC_LIBRARIES}
        simmap
        )

# include directory
target_include_directories(mapserver PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src
        )