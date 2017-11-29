
cmake_minimum_required(VERSION 3.8)
project(starflow_analytics)

set(CMAKE_CXX_STANDARD 14) # raft requires C++14
set(CMAKE_CXX_FLAGS -DL1D_CACHE_LINE_SIZE=64) # 64 bytes for x86_64 -- adapt for used arch

include(cmake/common.cmake)

include(cmake/dep/protobuf.cmake)
include(cmake/dep/pcap.cmake)
include(cmake/dep/raft.cmake)
include(cmake/dep/catch.cmake)

file(MAKE_DIRECTORY ${CMAKE_HOME_DIRECTORY}/src/proto)

add_custom_command(
        OUTPUT
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.h"
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.cc"
        COMMAND protoc
        ARGS --cpp_out=src/proto starflow.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

#include(cmake/exe/clfr_converter.cmake)
#include(cmake/exe/clfr_driver.cmake)

include(cmake/exe/pcap_flow_file_exporter.cmake)
include(cmake/exe/packet_capture.cmake)
include(cmake/exe/test_runner.cmake)