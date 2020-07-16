# message
message(STATUS "Enabling protobuf")

# find protobuf
find_package(Protobuf REQUIRED)

# include directories
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
