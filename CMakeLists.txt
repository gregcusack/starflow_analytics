cmake_minimum_required(VERSION 3.8)
project(starflow_analytics)

set(CMAKE_CXX_STANDARD 14) # raft requires C++14
set(CMAKE_CXX_FLAGS -DL1D_CACHE_LINE_SIZE=64) # 64 bytes for x86_64 -- adapt for used arch

include(cmake/protobuf.cmake)
include(cmake/pcap.cmake)
include(cmake/cpp_redis.cmake)
include(cmake/raft.cmake)

add_custom_command(
        OUTPUT
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.h"
            "${CMAKE_CURRENT_LIST_DIR}/src/proto/starflow.pb.cc"
        COMMAND protoc
        ARGS --cpp_out=src/proto starflow.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)


# test_publisher
add_executable(test_publisher src/test_publisher_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_client.h src/redis_client.cc
        src/redis_flow_publisher.h src/redis_flow_publisher.cc)

target_link_libraries(test_publisher protobuf)
target_link_libraries(test_publisher cpp_redis)
target_link_libraries(test_publisher tacopie)


# test_subscriber
add_executable(test_subscriber src/test_subscriber_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(test_subscriber protobuf)
target_link_libraries(test_subscriber cpp_redis)
target_link_libraries(test_subscriber tacopie)


# raft_example
add_executable(raft_example src/raft_example_main.cc
        src/kernels/counter.h
        src/kernels/printer.h
        src/kernels/redis_flow_subscriber.h
        src/kernels/ip_source_timestamps.h
        src/kernels/sink.h
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(raft_example protobuf)
target_link_libraries(raft_example cpp_redis)
target_link_libraries(raft_example tacopie)
target_link_libraries(raft_example raft)


# pcap_redis_publisher
add_executable(pcap_redis_publisher
        src/pcap_redis_publisher_main.cc
        src/redis_flow_publisher.cc
        src/redis_client.cc
        src/flow_table.cc
        src/flow.cc
        src/proto/starflow.pb.cc
        src/import/pcap_replay.cc src/flow_file_redis_publisher_main.cc)

target_link_libraries(pcap_redis_publisher protobuf)
target_link_libraries(pcap_redis_publisher cpp_redis)
target_link_libraries(pcap_redis_publisher pcap)
target_link_libraries(pcap_redis_publisher tacopie)


# pcap_flow_exporter
add_executable(pcap_flow_file_exporter
        src/pcap_flow_file_exporter_main.cc
        src/flow_table.cc
        src/flow.cc
        src/proto/starflow.pb.cc
        src/import/pcap_replay.cc)

target_link_libraries(pcap_flow_file_exporter protobuf)
target_link_libraries(pcap_flow_file_exporter pcap)


# flow_file_redis_publisher
add_executable(flow_file_redis_publisher
        src/flow_file_redis_publisher_main.cc
        src/redis_flow_publisher.cc
        src/redis_client.cc
        src/proto/starflow.pb.cc
        src/flow.cc
        src/flow_table.cc
        src/import/flow_file_replay.cc)

target_link_libraries(flow_file_redis_publisher protobuf)
target_link_libraries(flow_file_redis_publisher cpp_redis)
target_link_libraries(flow_file_redis_publisher tacopie)
