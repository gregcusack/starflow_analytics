cmake_minimum_required(VERSION 3.8)
project(starflow_analytics)

set(CMAKE_CXX_STANDARD 11)

include(cmake/protobuf.cmake)
include(cmake/pcap.cmake)
include(cmake/cpp_redis.cmake)

add_custom_command(
        OUTPUT
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.h"
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.cc"
        COMMAND protoc
        ARGS --cpp_out=src/proto starflow.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(test_exe test.cc src/proto/starflow.pb.cc)
target_link_libraries(test_exe protobuf)