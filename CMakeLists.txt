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


add_executable(test_publisher src/test_publisher_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_client.h src/redis_client.cc
        src/redis_flow_publisher.h src/redis_flow_publisher.cc)

target_link_libraries(test_publisher protobuf)
target_link_libraries(test_publisher cpp_redis)
target_link_libraries(test_publisher tacopie)


add_executable(test_subscriber src/test_subscriber_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(test_subscriber protobuf)
target_link_libraries(test_subscriber cpp_redis)
target_link_libraries(test_subscriber tacopie)

add_executable(raft_example src/raft_example_main.cc
        src/flow.h src/flow.cc
        src/flow_table.h src/flow_table.cc
        src/proto/starflow.pb.h src/proto/starflow.pb.cc
        src/redis_subscriber.h src/redis_subscriber.cc
        src/redis_flow_subscriber.h src/redis_flow_subscriber.cc)

target_link_libraries(raft_example protobuf)
target_link_libraries(raft_example cpp_redis)
target_link_libraries(raft_example tacopie)
target_link_libraries(raft_example raft)