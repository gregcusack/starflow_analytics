
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

set(MODULES_DIR src/modules)
set(MODULES_SRC
        ${MODULES_DIR}/raw_packet_parser.h ${MODULES_DIR}/raw_packet_parser.cc
        ${MODULES_DIR}/pcap_reader.h ${MODULES_DIR}/pcap_reader.cc)

set(TYPES_DIR src/types)
set(TYPES_SRC
        ${TYPES_DIR}/clfr.h ${TYPES_DIR}/clfr.cc
        ${TYPES_DIR}/features.h ${TYPES_DIR}/features.cc
        ${TYPES_DIR}/key.h ${TYPES_DIR}/key.cc
        ${TYPES_DIR}/packet.h ${TYPES_DIR}/packet.cc
        ${TYPES_DIR}/raw_packet.h ${TYPES_DIR}/raw_packet.cc)

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